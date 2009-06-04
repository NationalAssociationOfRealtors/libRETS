/*
 * Copyright (C) 2008 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */

#include <sstream>
#include <cerrno>
#include <iostream>
#include "librets/CurlMulti.h"
#include "librets/CurlEasy.h"
#include "librets/CurlHttpResponse.h"
#include "librets/RetsException.h"
#include "librets/str_stream.h"

using namespace librets;
using std::string;
using std::ostringstream;

#define CLASS CurlMulti

void CLASS::CurlAssert(CURLMcode errorCode, const string & prefix)
{
    if (errorCode != CURLM_OK)
    {
        const char * curlError = curl_multi_strerror(errorCode);
        ostringstream message;
        if (!prefix.empty())
        {
            message << "Could not " << prefix << ": ";
        }
        message << "curl error #" << errorCode  << " (" << curlError
            << ")";
        throw RetsException(message.str());
    }
}

CLASS::CLASS()
{
    mMultiHandle = curl_multi_init();
    if (!mMultiHandle)
    {
        throw RetsException("Could not allocate Curl multi handle");
    }
    mShareHandle = curl_share_init();
    if (!mShareHandle)
    {
        throw RetsException("Could not allocate Curl shared handle");
    }
    curl_share_setopt(mShareHandle, CURLSHOPT_SHARE, CURL_LOCK_DATA_COOKIE);
}

CLASS::~CLASS()
{
    /*
     * Destroy the contents of the CurlEasy cache. We need to manually do this
     * instead of letting the automatic destruction happen in this case because
     * the CurlEasy objects must no longer be referenced before we call
     * curl_share_cleanup().
     */
    for (CurlEasyVector::iterator i = mCurlEasyAvailable.begin(); 
         i != mCurlEasyAvailable.end(); i++)
    {
        i->reset();
    }
    curl_multi_cleanup(mMultiHandle);
    curl_share_cleanup(mShareHandle);
}

void CLASS::AddEasy(CurlEasyPtr curlEasy)
{
    CurlAssert(curl_multi_add_handle(mMultiHandle, curlEasy->GetEasyHandle()));
    mCurlEasyInUse.push_back(curlEasy);
}

CurlEasyPtr CLASS::EasyFactory()
{
    CurlEasyPtr curlEasy;
    /*
     * See if the cache is empty. If so, create a new one, otherwise use the
     * last one. We intentionally try to use the last one in the queue in order
     * to try to maintain the connection and not have to go through the authentication
     * process for each request.
     */
    if (!mCurlEasyAvailable.empty())
    {
        curlEasy = mCurlEasyAvailable.back();
        mCurlEasyAvailable.pop_back();
    }
    else
    {
        curlEasy.reset(new CurlEasy());
        curlEasy->SetShareHandle(mShareHandle);
    }

    return curlEasy;
}

void CLASS::FreeEasy(CurlEasyPtr curlEasy)
{
    mCurlEasyAvailable.push_back(curlEasy);
}

void CLASS::RemoveEasy(CurlEasyPtr curlEasy)
{
    CurlAssert(curl_multi_remove_handle(mMultiHandle, curlEasy->GetEasyHandle()));            
}

CURLM * CLASS::GetMultiHandle()
{
    return mMultiHandle;
}

/*
 * Use curl_multi_perform to process an http transaction. This (through
 * the curl callbacks), will load data into a stream for consumption
 * through CurlStream. This is only ever intended to be called from
 * CurlHttpClient::ContinueRequest(). As a side effect, the curl flag
 * that indicates that the transaction continues (stored in mStillRunning)
 * is updated.
 */
void CLASS::Perform()
{
    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int maxfd               = 0;
    CURLMcode cres;
    int rc                  = 0;
    struct timeval timeout;
    long wait               = 0;
    
    if(!mMultiHandle)
    {
        throw RetsException("No multi handle has been allocated prior to the Perform() call.");
    }

    /*
     * See how long curl thinks we should wait before continuing.
     */
    curl_multi_timeout(mMultiHandle, &wait);
    if (wait == -1) 
        wait = 1;
   
    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);
    
    timeout.tv_sec = wait / 1000;
    timeout.tv_usec = (wait % 1000) * 1000;
    
    cres = curl_multi_fdset(mMultiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
    
    if (cres != CURLM_OK)
    {
        throw RetsException("curl_multi_fdset failed.");
    }
    
    /*
     * See if one of the curl handles need service.
     */
    if (maxfd > -1)
        rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);

    if (rc < 0)
    {
        ostringstream message;
        message << "CurlMulti::Perform - select failed: " << errno;
        throw RetsException(message.str());
    }

    /*
     * The libCURL docs seem to indicate that we should periodically call
     * curl_multi_perform even if we don't have activity. This will also launch
     * the first I/O if none has already been started.
     *
     * When done here, we'll return and let the code
     * elsewhere handle deblocking and serving it. We will re-enter this
     * routine only when a request for data exceeds what remains in the
     * stream.
     */
    while (curl_multi_perform(mMultiHandle, &mStillRunning) == CURLM_CALL_MULTI_PERFORM);

    /*
     * Check status.
     */
    CURLMsg * msg;
    int remaining = 0;
        
    while ((msg = curl_multi_info_read(mMultiHandle, &remaining)))
    {
        if (msg->msg == CURLMSG_DONE)
        {
            /* 
             * See if we can find the handle in the vector and then delete it.
             */
            for (CurlEasyVector::iterator i = mCurlEasyInUse.begin(); i != mCurlEasyInUse.end(); i++)
            {
                if ((*i)->GetEasyHandle() == msg->easy_handle)
                {
                    CurlHttpClientPrivate * client = (CurlHttpClientPrivate *)(*i)->GetPrivateData();
                    CurlHttpResponsePtr response;
                    if (client && (response = client->GetResponse()))
                    {
                        if (msg->data.result == CURLE_OPERATION_TIMEDOUT)
                        {
                            response->SetResponseCode(408);
                            response->SetAdditionalErrorText("cURL reports an operation timeout.");
                        }
                        else
                        if (msg->data.result == CURLE_COULDNT_CONNECT)
                        {
                            response->SetResponseCode(404);
                            response->SetAdditionalErrorText("cURL reports unable to connect.");
                        }
                        else
                        if (msg->data.result == CURLE_COULDNT_RESOLVE_HOST)
                        {
                            response->SetResponseCode(404);
                            response->SetAdditionalErrorText(str_stream() <<
                                                "cURL reports unable to resolve host: " <<
                                                (*i)->GetUrl());
                        }
                        else
                        if (msg->data.result == CURLE_COULDNT_RESOLVE_PROXY)
                        {
                            response->SetResponseCode(404);
                            response->SetAdditionalErrorText("cURL reports unable to resolve proxy.");
                        }
                        else
                        if (msg->data.result == CURLE_UNSUPPORTED_PROTOCOL)
                        {
                            response->SetResponseCode(503);
                            response->SetAdditionalErrorText("cURL reports that the protocol is unsupported.");
                        }
                        else
                            response->SetResponseCode((*i)->GetResponseCode());
                        response->SetInProgress(false);
                        delete client;
                    }
                    FreeEasy(*i);
                    curl_multi_remove_handle(mMultiHandle, msg->easy_handle);
                    mCurlEasyInUse.erase(i);
                    break;
                }
            }
        }
    }

    mPriorRunning = mStillRunning;
}

curl_slist * CLASS::GetCookieSlist()
{
    return mCookieList;
}

bool CLASS::StillRunning()
{
    return mStillRunning > 0;
}

int CLASS::GetPriorRunning()
{
    return mPriorRunning;
}

int CLASS::GetStillRunning()
{
    return mStillRunning;
}

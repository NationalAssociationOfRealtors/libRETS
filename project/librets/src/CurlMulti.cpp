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
#include "librets/CurlMulti.h"
#include "librets/CurlEasy.h"
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
    Reset();
}

CLASS::~CLASS()
{
    curl_multi_cleanup(mMultiHandle);
}

void CLASS::AddEasy(CurlEasy & curlEasy)
{
    CurlAssert(curl_multi_add_handle(mMultiHandle, curlEasy.GetEasyHandle()));
}

void CLASS::RemoveEasy(CurlEasy & curlEasy)
{
    CurlAssert(curl_multi_remove_handle(mMultiHandle, curlEasy.GetEasyHandle()));            
}

CURLM * CLASS::GetMultiHandle()
{
    return mMultiHandle;
}

/**
 * Perform a multi operation. The caller will need to handle the determination as
 * to whether enough data has arrived and if not, call Perform() again in a loop.
 */
void CLASS::Perform()
{
    fd_set fdread;
    fd_set fdwrite;
    fd_set fdexcep;
    int maxfd;
    CURLMcode cres;
    int rc;
    struct timeval timeout;
    
    if(!mMultiHandle)
    {
        throw RetsException("No multi handle has been allocated prior to the Perform() call.");
    }
    
    if (!mStillRunning)
        return;
        
    /*
     * If mStillRunning is 2, we need to prime the pump.
     */
    if (mStillRunning == 2)
    {
        while (curl_multi_perform(mMultiHandle, &mStillRunning) == CURLM_CALL_MULTI_PERFORM);
    }
     
    FD_ZERO(&fdread);
    FD_ZERO(&fdwrite);
    FD_ZERO(&fdexcep);
    
    timeout.tv_sec = 15;
    timeout.tv_usec = 0;
    
    cres = curl_multi_fdset(mMultiHandle, &fdread, &fdwrite, &fdexcep, &maxfd);
    
    if (cres != CURLM_OK)
    {
        throw RetsException("curl_multi_fdset failed.");
    }
    
    rc = select(maxfd + 1, &fdread, &fdwrite, &fdexcep, &timeout);
    
    if (rc > 0)
    {
        while (curl_multi_perform(mMultiHandle, &mStillRunning) == CURLM_CALL_MULTI_PERFORM);
    }
    else
    if (rc < 0)
    {
        ostringstream message;
        message << "CurlMulti::Perform - select failed: " << errno;
        throw RetsException(message.str());
    }
}

/**
 * Reset for a new transaction.
 */
void CLASS::Reset()
{
    mStillRunning = 2;
}

bool CLASS::StillRunning()
{
    return mStillRunning > 0;
}

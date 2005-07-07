/*
 * Copyright (C) 2005 National Association of REALTORS(R)
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
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "librets/RetsHttpRequest.h"
#include "librets/CurlHttpClient.h"
#include "librets/CurlHttpResponse.h"
#include "librets/RetsException.h"
#include "librets/RetsHttpLogger.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using namespace std;
namespace ba = boost::algorithm;

#define CURL_ASSERT(_X_) CurlAssert(LIBRETS_ECTXT(), _X_)

CurlHttpClient::CurlHttpClient()
{
    mCurl = curl_easy_init();
    if (!mCurl)
    {
        throw RetsException("Could not allocate Curl handle");
    }

    try
    {
        mLogger = NullHttpLogger::GetInstance();
        CurlAssert(
            LIBRETS_ECTXT(),
            curl_easy_setopt(mCurl, CURLOPT_ERRORBUFFER, mCurlErrorBuffer),
            false);
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_VERBOSE, false));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_DEBUGFUNCTION,
                                     CurlHttpClient::StaticDebug));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_DEBUGDATA, this));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HTTPAUTH, CURLAUTH_ANY));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_COOKIEFILE, ""));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, this));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION,
                                     CurlHttpClient::StaticWriteData));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_WRITEHEADER, this));
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HEADERFUNCTION,
                                     CurlHttpClient::StaticWriteHeader));
    }
    catch (RetsException &)
    {
        curl_easy_cleanup(mCurl);
        throw;
    }
}

CurlHttpClient::~CurlHttpClient()
{
    curl_easy_cleanup(mCurl);
}

void CurlHttpClient::CurlAssert(const RetsExceptionContext & context,
                                CURLcode errorCode, bool useErrorBuffer)
{
    if (errorCode != CURLE_OK)
    {
        const char * curlError = curl_easy_strerror(errorCode);
        ostringstream message;
        message << "curl error #" << errorCode  << " (" << curlError
                << ")";
        if (useErrorBuffer)
        {
            message << ": " << (char *)(mCurlErrorBuffer);
        }
        RetsException e(message.str());
        e.SetContext(context);
        throw e;
    }
}

void CurlHttpClient::SetDefaultHeader(string name, string value)
{
    mDefaultHeaders[name] = value;
    GenerateHeaderSlist();
}

void CurlHttpClient::ClearDefaultHeader(string name)
{
    mDefaultHeaders.erase(name);
    GenerateHeaderSlist();
}

void CurlHttpClient::GenerateHeaderSlist()
{
    mHeaders.free_all();
    StringMap::const_iterator i;
    for (i = mDefaultHeaders.begin(); i != mDefaultHeaders.end(); i++)
    {
        string header = (*i).first + ": " + (*i).second;
        mHeaders.append(header.c_str());
    }
}

void CurlHttpClient::SetUserAgent(string userAgent)
{
    SetDefaultHeader("User-Agent", userAgent);
}

void CurlHttpClient::SetUserCredentials(string userName, string password)
{
    mCurlUserpwd = userName + ":" + password;
    CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_USERPWD,
                                 mCurlUserpwd.c_str()));
}

RetsHttpResponsePtr CurlHttpClient::DoRequest(RetsHttpRequestPtr request)
{
    mCurlUrl = request->GetUrl();
    mQueryString = request->GetQueryString();
    if (request->GetMethod() == RetsHttpRequest::GET)
    {
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HTTPGET, 1));
        if (mQueryString != "")
        {
            mCurlUrl = mCurlUrl + "?" + mQueryString;
        }
    }
    else
    {
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_POSTFIELDS,
                                     mQueryString.c_str()));
    }
    CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, mHeaders.slist()));
    CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_URL, mCurlUrl.c_str()));
    mResponse.reset(new CurlHttpResponse());
    iostreamPtr dataStream(new stringstream());
    mResponse->SetStream(dataStream);
    CURL_ASSERT(curl_easy_perform(mCurl));
    mResponse->SetUrl(request->GetUrl());
    long responseCode;
    CURL_ASSERT(
        curl_easy_getinfo(mCurl, CURLINFO_RESPONSE_CODE, &responseCode));
    mResponse->SetResponseCode(responseCode);
    return mResponse;
}

void CurlHttpClient::SetLogger(RetsHttpLogger * logger)
{
    if (logger == 0)
    {
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_VERBOSE, false));
        mLogger = NullHttpLogger::GetInstance();
    }
    else
    {
        mLogger = logger;
        CURL_ASSERT(curl_easy_setopt(mCurl, CURLOPT_VERBOSE, true));
    }
}

size_t CurlHttpClient::StaticWriteData(void * buffer, size_t size, size_t nmemb,
                                       void * userData)
{
    CurlHttpClient * client = (CurlHttpClient *) userData;
    return client->WriteData(buffer, size, nmemb);
}

size_t CurlHttpClient::WriteData(void * buffer, size_t size, size_t nmemb)
{
    size_t bytes = size * nmemb;
    mResponse->WriteData((const char *) buffer, bytes);
    return bytes;
}

size_t CurlHttpClient::StaticWriteHeader(void * buffer, size_t size,
                                         size_t nmemb, void * userData)
{
    CurlHttpClient * client = (CurlHttpClient *) userData;
    return client->WriteHeader(buffer, size, nmemb);
}

size_t CurlHttpClient::WriteHeader(void * buffer, size_t size, size_t nmemb)
{
    size_t bytes = size * nmemb;

    string header((const char *) buffer, bytes);
    string name;
    string value;
    if (splitField(header, ":", name, value))
    {
        ba::trim(value);
        mResponse->SetHeader(name, value);
    }

    return bytes;
}

int CurlHttpClient::StaticDebug(CURL * handle, curl_infotype type, char * data,
                                size_t size, void * userData)
{
    CurlHttpClient * client = (CurlHttpClient *) userData;
    return client->Debug(handle, type, data, size);
}

int CurlHttpClient::Debug(CURL * handle, curl_infotype type, char * data,
                          size_t size)
{
    string text(data, size);
    switch (type)
    {
        case CURLINFO_TEXT:
            mLogger->logHttpData(RetsHttpLogger::INFORMATIONAL, text);
            break;
            
        case CURLINFO_HEADER_IN:
        case CURLINFO_DATA_IN:
        case CURLINFO_SSL_DATA_IN:
            mLogger->logHttpData(RetsHttpLogger::RECEIVED, text);
            break;
            
        case CURLINFO_HEADER_OUT:
        case CURLINFO_DATA_OUT:
        case CURLINFO_SSL_DATA_OUT:
            mLogger->logHttpData(RetsHttpLogger::SENT, text);
            break;
            
        default:
            // Ignore
            break;
    }
    return 0;
}

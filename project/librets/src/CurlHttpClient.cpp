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

CurlHttpClient::CurlHttpClient()
{
    mLogger = NullHttpLogger::GetInstance();
    mCurl.SetVerbose(false);
    mCurl.SetDebugData(this);
    mCurl.SetDebugFunction(CurlHttpClient::StaticDebug);
    mCurl.SetHttpAuth(CURLAUTH_ANY);
    mCurl.SetCookieFile("");
    mCurl.SetWriteData(this);
    mCurl.SetWriteFunction(CurlHttpClient::StaticWriteData);
    mCurl.SetWriteHeaderData(this);
    mCurl.SetWriteHeaderFunction(CurlHttpClient::StaticWriteHeader);
    SetUserAgent("librets-curl/" LIBRETS_VERSION);
}

void CurlHttpClient::SetDefaultHeader(string name, string value)
{
    mDefaultHeaders[name] = value;
}

string CurlHttpClient::GetDefaultHeader(string name) const
{
    StringMap::const_iterator i = mDefaultHeaders.find(name);
    if (i != mDefaultHeaders.end())
        return i->second;
    else
        return "";
}

void CurlHttpClient::ClearDefaultHeader(string name)
{
    mDefaultHeaders.erase(name);
}

void CurlHttpClient::GenerateHeadersSlist(const StringMap & requestHeaders)
{
    // Start by copying default headers, then add request headers
    StringMap allHeaders = mDefaultHeaders;
    allHeaders.insert(requestHeaders.begin(), requestHeaders.end());

    mHeaders.free_all();
    StringMap::const_iterator i;
    for (i = allHeaders.begin(); i != allHeaders.end(); i++)
    {
        string header = (*i).first + ": " + (*i).second;
        mHeaders.append(header.c_str());
    }
}

void CurlHttpClient::SetUserAgent(string userAgent)
{
    SetDefaultHeader("User-Agent", userAgent);
}

string CurlHttpClient::GetUserAgent() const
{
    return "";
}

void CurlHttpClient::SetUserCredentials(string userName, string password)
{
    mCurl.SetUserCredentials(userName, password);
}

RetsHttpResponsePtr CurlHttpClient::DoRequest(RetsHttpRequest * request)
{
    string url = request->GetUrl();
    string queryString = request->GetQueryString();
    if (request->GetMethod() == RetsHttpRequest::GET)
    {
        mCurl.SetHttpGet(true);
        if (!queryString.empty())
        {
            url += "?" + queryString;  
        }
    }
    else
    {
        mCurl.SetPostFields(queryString);
    }
    GenerateHeadersSlist(request->GetHeaderMap());
    mCurl.SetHttpHeaders(mHeaders.slist());
    mCurl.SetUrl(url);
    mResponse.reset(new CurlHttpResponse());
    iostreamPtr dataStream(new stringstream());
    mResponse->SetStream(dataStream);
    mCurl.Perform();
    mResponse->SetUrl(request->GetUrl());
    mResponse->SetResponseCode(mCurl.GetResponseCode());
    return mResponse;
}

void CurlHttpClient::SetLogger(RetsHttpLogger * logger)
{
    if (logger == 0)
    {
        mCurl.SetVerbose(false);
        mLogger = NullHttpLogger::GetInstance();
    }
    else
    {
        mLogger = logger;
        mCurl.SetVerbose(true);
    }
}

RetsHttpLogger* CurlHttpClient::GetLogger() const
{
    return mLogger;
}

size_t CurlHttpClient::StaticWriteData(char * buffer, size_t size, size_t nmemb,
                                       void * userData)
{
    CurlHttpClient * client = (CurlHttpClient *) userData;
    return client->WriteData(buffer, size, nmemb);
}

size_t CurlHttpClient::WriteData(char * buffer, size_t size, size_t nmemb)
{
    size_t bytes = size * nmemb;
    mResponse->WriteData(buffer, bytes);
    return bytes;
}

size_t CurlHttpClient::StaticWriteHeader(char * buffer, size_t size,
                                         size_t nmemb, void * userData)
{
    CurlHttpClient * client = (CurlHttpClient *) userData;
    return client->WriteHeader(buffer, size, nmemb);
}

size_t CurlHttpClient::WriteHeader(char * buffer, size_t size, size_t nmemb)
{
    size_t bytes = size * nmemb;

    string header(buffer, bytes);
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

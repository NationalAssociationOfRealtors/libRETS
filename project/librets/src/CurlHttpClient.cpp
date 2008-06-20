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
#include "librets/CurlStream.h"
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
    
    mCurlMulti.AddEasy(mCurl);
    
    mResponseCode = 0;

    SetUserAgent("librets-curl/" LIBRETS_VERSION);
}

CurlHttpClient::~CurlHttpClient()
{
    mCurlMulti.RemoveEasy(mCurl);
}

string CurlHttpClient::GetCookie(const char * name)
{
    string cookie(name);
    
    mCookies.free_all();
    mCookies.set(mCurl.GetCookieSlist());
    
    curl_slist *slist = (curl_slist *)mCookies.slist();
    
    if (!slist)
    {
        return "";
    }
    
    /*
     * Each data entry in the slist is in Netscape Format. The fields are:
     *    domain    The domain name
     *    flag      TRUE/FALSE indicating whether all domains can access
     *    path      The path in the domain for which the cookie is valid
     *    secure    TRUE/FALSE indicating whether or not a secure connection needed
     *    expiration The expiration in seconds since 1/1/1970 00:00:00 GMT
     *    name      The name of the cookie
     *    value     The value of the cookie
     */
    
    while (slist)
    {
        StringVector values;
        StringVector::const_iterator i;
    
        ba::split(values, slist->data, ba::is_any_of("\t"));

        string key = values[5];
        string value = values[6];
        ba::trim(key);
        ba::trim(value);

        if (cookie == key)
        {
            return value;
        }
        slist = slist->next;
    }
    return "";
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

int CurlHttpClient::GetResponseCode()
{
    /*
     * With the multi interface, we won't get status until the transaction is done.
     * Here we assume that if someone wants status, they want the request completed, so
     * complete it.
     */
    while (mResponseCode == 0 && ContinueRequest());
    
    return mResponseCode;
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

/**
 * Continue with the request if not completed.
 * @return boolean that indicates whether or not the transaction has finished.
 */
bool CurlHttpClient::ContinueRequest()
{
    if (mCurlMulti.StillRunning())
        mCurlMulti.Perform();
        
    if (!mCurlMulti.StillRunning())
    {
        mResponseCode = mCurl.GetResponseCode();
    }
        
    return mCurlMulti.StillRunning();
}

RetsHttpResponsePtr CurlHttpClient::StartRequest(RetsHttpRequest * request)
{
    string url = request->GetUrl();
    string queryString = request->GetQueryString();
    bool wasVerbose = mCurl.GetVerbose();
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
    iostreamPtr dataStream(new CurlStream(*this));
    mResponse->SetStream(dataStream);
    mResponse->SetHttpClient(this);
    /*
     * It appears that to start a new transaction, we must remove and then
     * add the Easy handle back.
     */
    mCurlMulti.RemoveEasy(mCurl);
    mCurlMulti.AddEasy(mCurl);
    mCurlMulti.Reset();
    /*
     * Start the html request. This will return immediately.
     */
    mCurlMulti.Perform();
    
    mCurl.SetVerbose(wasVerbose);
    mResponse->SetUrl(request->GetUrl());
    mResponseCode = mCurl.GetResponseCode();
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

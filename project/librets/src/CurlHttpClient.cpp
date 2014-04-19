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
#include "librets/RetsSession.h"
#include "librets/str_stream.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using namespace std;
namespace ba = boost::algorithm;

CurlHttpClient::CurlHttpClient()
{
    CurlEasy Curl;
    /*
     * The libCURL docs indicate that with SSL support, we need to make
     * sure curl_global_init is called. The easiest way is to tell libCURL
     * to initialize all modules that are built.
     */
    curl_global_init(CURL_GLOBAL_ALL);

    mFlags = 0;
    mLogger = NullHttpLogger::GetInstance();
    mLogging = false;
    mTimeout = 0;

    SetUserAgent("librets-curl/" LIBRETS_VERSION);
    SetDefaultHeader("X-Librets-Version", LIBRETS_VERSION ", libCURL - " + Curl.GetVersion());
}

CurlHttpClient::~CurlHttpClient()
{
    /*
     * Some cURL cleanup won't happen until until final exit. But since the calling
     * program may be long lived, let's get cURL to cleanup as much of the transient 
     * stuff allocated for this transaction.
     */
    curl_global_cleanup();
}

string CurlHttpClient::GetCookie(const char * name)
{
    string cookie(name);
    
    CurlEasyPtr curlEasy  = mCurlMulti.EasyFactory();

    /*
     * We probably do not need the easy handle fully populated since all
     * we want to do is fetch the cookies.
     */
    curlEasy->SetHttpAuth(CURLAUTH_ANY);
    curlEasy->SetCookieFile("");
    curlEasy->SetUserCredentials(mUserName, mPassword);

    if (!mProxyUrl.empty())
    {
        curlEasy->SetProxyUrl(mProxyUrl);
        if (!mProxyPassword.empty())
            curlEasy->SetProxyPassword(mProxyPassword);
    }

    curlEasy->SetUrl(mUrl);

    curl_slist *slist = curlEasy->GetCookieSlist();

    mCurlMulti.FreeEasy(curlEasy);
    
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
    mUserName = userName;
    mPassword = password;
}

void CurlHttpClient::SetProxy(string url, string password)
{
    ba::trim(url);
    ba::trim(password);
    mProxyUrl = url;
    mProxyPassword = password;
}

void CurlHttpClient::SetTimeout(int seconds)
{
    mTimeout = seconds;
}

/**
 * Continue with the request if not completed.
 * @return boolean that indicates whether or not the transaction has finished.
 */
bool CurlHttpClient::ContinueRequest()
{
    /*
     * If there is at least one easy handle registered and running, we need to
     * invoke the Perform() loop.
     */
    if (mCurlMulti.StillRunning())
        mCurlMulti.Perform();
        
    return mCurlMulti.StillRunning();
}

RetsHttpResponsePtr CurlHttpClient::StartRequest(RetsHttpRequest * request)
{
    CurlEasyPtr         curlEasy  = mCurlMulti.EasyFactory();
    CurlHttpResponsePtr response(new CurlHttpResponse());
    CurlHttpClientPrivate  *client =
        new CurlHttpClientPrivate(request, response, this, curlEasy);
    
    if (curlEasy == NULL)
    {
        throw RetsException(str_stream() << "Unable to create CurlEasy handle");
    }
    
    if (client == NULL)
    {
        throw RetsException(str_stream() << "Unable to create Curl Private data structure");
    }

    /*
     * Initialize the object for this request.
     */
    curlEasy->SetDebugData(client);
    curlEasy->SetDebugFunction(CurlHttpClient::StaticDebug);
    curlEasy->SetHttpAuth(CURLAUTH_ANY);
    curlEasy->SetCookieFile("");
    curlEasy->SetTimeout(mTimeout);
    curlEasy->SetUserCredentials(mUserName, mPassword);
    curlEasy->SetWriteData(client);
    curlEasy->SetWriteFunction(CurlHttpClient::StaticWriteData);
    curlEasy->SetWriteHeaderData(client);
    curlEasy->SetWriteHeaderFunction(CurlHttpClient::StaticWriteHeader);
    
    if (mFlags & RetsSession::MODE_NO_SSL_VERIFY)
        curlEasy->SetSSLVerify(false);
    else
        curlEasy->SetSSLVerify(true);

    string url = request->GetUrl();
    if (url.empty())
    {
        throw RetsException(str_stream() << "Invalid operation: null URL. Check login capability URL list.");
    }
    
    mUrl = url;
    string queryString = request->GetQueryString();
    if (mLogging)
        curlEasy->SetVerbose(request->GetLogging());

    if (!mProxyUrl.empty())
    {
        curlEasy->SetProxyUrl(mProxyUrl);
        if (!mProxyPassword.empty())
            curlEasy->SetProxyPassword(mProxyPassword);
    }

    if (request->GetMethod() == RetsHttpRequest::GET)
    {
        curlEasy->SetHttpGet(true);
        if (!queryString.empty())
        {
            url += "?" + queryString;  
        }
    }
    else
    {
        curlEasy->SetPostFields(queryString);
    }
    GenerateHeadersSlist(request->GetHeaderMap());
    curlEasy->SetHttpHeaders(mHeaders.slist());
    curlEasy->SetUrl(url);
    
    /*
     * Register the client object with the easy handle as its private data.
     * This will allow the CurlMulti handling loop to determine the proper context
     * for that handle.
     */
    curlEasy->SetPrivateData(client);
    
    iostreamPtr dataStream(new CurlStream(*this));
    response->SetStream(dataStream);
    response->SetHttpClient(this);
    response->SetHttpRequest(request);

    mCurlMulti.AddEasy(curlEasy);
    /*
     * Start the html request. This will return immediately.
     */
    mCurlMulti.Perform();
    
    return response;
}

void CurlHttpClient::SetLogger(RetsHttpLogger * logger)
{
    if (logger == 0)
    {
        mLogger = NullHttpLogger::GetInstance();
        mLogging = false;
    }
    else
    {
        mLogger = logger;
        mLogging = true;
    }
}

void CurlHttpClient::SetModeFlags(unsigned int flags)
{
    mFlags = flags;
}

RetsHttpLogger* CurlHttpClient::GetLogger() const
{
    return mLogger;
}

size_t CurlHttpClient::StaticWriteData(char * buffer, size_t size, size_t nmemb,
                                       void * userData)
{
    CurlHttpClientPrivate * client = (CurlHttpClientPrivate *) userData;
    client->GetResponse()->SetResponseCode(
        client->GetCurlEasy()->GetResponseCode()
    );
    size_t bytes = size * nmemb;
    client->GetResponse()->WriteData(buffer, bytes);
    return bytes;
}

size_t CurlHttpClient::StaticWriteHeader(char * buffer, size_t size,
                                         size_t nmemb, void * userData)
{
    CurlHttpClientPrivate * client = (CurlHttpClientPrivate *) userData;

    size_t bytes = size * nmemb;

    string header(buffer, bytes);
    string name;
    string value;
    if (splitField(header, ":", name, value))
    {
        ba::trim(value);
        client->GetResponse()->SetHeader(name, value);
    }


    return bytes;
}

int CurlHttpClient::StaticDebug(CURL * handle, curl_infotype type, char * data,
                                size_t size, void * userData)
{
    CurlHttpClientPrivate * client = (CurlHttpClientPrivate *) userData;
    string text(data, size);
    switch (type)
    {
        case CURLINFO_TEXT:
            client->GetClient()->GetLogger()->logHttpData(RetsHttpLogger::INFORMATIONAL, text);
            break;
            
        case CURLINFO_HEADER_IN:
        case CURLINFO_DATA_IN:
        case CURLINFO_SSL_DATA_IN:
            client->GetClient()->GetLogger()->logHttpData(RetsHttpLogger::RECEIVED, text);
            break;
            
        case CURLINFO_HEADER_OUT:
        case CURLINFO_DATA_OUT:
        case CURLINFO_SSL_DATA_OUT:
            client->GetClient()->GetLogger()->logHttpData(RetsHttpLogger::SENT, text);
            break;
            
        default:
            // Ignore
            break;
    }
    return 0;
}

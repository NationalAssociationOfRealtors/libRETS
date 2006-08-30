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
#include "librets/CurlEasy.h"
#include "librets/RetsException.h"
#include "librets/str_stream.h"

using namespace librets;
using std::string;
using std::ostringstream;

void CurlEasy::CurlAssert(CURLcode errorCode, const string & prefix)
{
    if (errorCode != CURLE_OK)
    {
        const char * curlError = curl_easy_strerror(errorCode);
        ostringstream message;
        if (!prefix.empty())
        {
            message << "Could not " << prefix << ": ";
        }
        message << "curl error #" << errorCode  << " (" << curlError
            << ")";
        if (mUseErrorBuffer)
        {
            message << ": " << mErrorBuffer;
        }
        throw RetsException(message.str());
    }
}

CurlEasy::CurlEasy()
    : mUseErrorBuffer(false)
{
    mCurl = curl_easy_init();
    if (!mCurl)
    {
        throw RetsException("Could not allocate Curl easy handle");
    }
    
    try
    {
        SetErrorBuffer(mErrorBuffer);
    }
    catch (RetsException &)
    {
        curl_easy_cleanup(mCurl);
        throw;
    }
}

CurlEasy::~CurlEasy()
{
    curl_easy_cleanup(mCurl);
}

void CurlEasy::SetErrorBuffer(char * errorBuffer)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_ERRORBUFFER, errorBuffer),
               "set error buffer");
    mUseErrorBuffer = true;
}

void CurlEasy::SetVerbose(bool verbose)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_VERBOSE, verbose ? 1 : 0),
               str_stream() << "set verbose to " << verbose);
}

void CurlEasy::SetDebugData(void * debugData)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_DEBUGDATA, debugData),
               "set debug data");
}

void CurlEasy::SetDebugFunction(curl_debug_callback debugFunction)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_DEBUGFUNCTION, debugFunction),
               "set debug function");
}

void CurlEasy::SetHttpAuth(long httpAuth)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_HTTPAUTH, httpAuth),
               "set HTTP auth");
}

void CurlEasy::SetCookieFile(std::string cookieFile)
{
    mCookieFile = cookieFile;
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_COOKIEFILE, mCookieFile.c_str()),
               "set cookie file");
}

void CurlEasy::SetWriteData(void * writeData)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_WRITEDATA, writeData),
               "set write data");
}

void CurlEasy::SetWriteFunction(curl_write_callback writeFunction)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_WRITEFUNCTION, writeFunction),
               "set write function");
}

void CurlEasy::SetWriteHeaderData(void * headerData)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_WRITEHEADER, headerData),
               "set header data");
}

void CurlEasy::SetWriteHeaderFunction(curl_write_callback headerFunction)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_HEADERFUNCTION, headerFunction),
               "set header function");
}

void CurlEasy::SetUserCredentials(std::string username, std::string password)
{
    mUserpwd = username + ":" + password;
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_USERPWD, mUserpwd.c_str()),
               "set user credentials");
}

void CurlEasy::SetHttpGet(bool httpGet)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_HTTPGET, httpGet ? 1 : 0),
               "set HTTP GET");
}

void CurlEasy::SetPostFields(std::string postFields)
{
    mPostFields = postFields;
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_POSTFIELDS, mPostFields.c_str()),
               "set post fields");
}

void CurlEasy::SetHttpHeaders(const curl_slist * httpHeaders)
{
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_HTTPHEADER, httpHeaders),
               "set HTTP headers");
}

void CurlEasy::SetUrl(string url)
{
    mUrl = url;
    CurlAssert(curl_easy_setopt(mCurl, CURLOPT_URL, mUrl.c_str()),
               "set url");
}

void CurlEasy::Perform()
{
    CurlAssert(curl_easy_perform(mCurl));
}

long CurlEasy::GetResponseCode()
{
    long responseCode;
    CurlAssert(curl_easy_getinfo(mCurl, CURLINFO_RESPONSE_CODE, &responseCode),
               "get response code");
    return responseCode;
}

CURL * CurlEasy::GetEasyHandle()
{
    return mCurl;
}

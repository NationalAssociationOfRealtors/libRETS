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
#include <iostream>
#include <boost/algorithm/string.hpp>
#include "librets/CurlHttpResponse.h"

using namespace librets;
using std::string;
namespace ba = boost::algorithm;

CurlHttpResponse::CurlHttpResponse()
                : mHttpClient()
                , mErrorText()
{
    mResponseCode = -1;
    mInProgress = true;
}   

CurlHttpResponse::~CurlHttpResponse()
{
}

void CurlHttpResponse::SetStream(iostreamPtr stream)
{
    mStream = stream;
}

void CurlHttpResponse::WriteData(const char * data, size_t bytes)
{
    mStream->write(data, bytes);
}

void CurlHttpResponse::SetUrl(string url)
{
}

int CurlHttpResponse::GetResponseCode() const
{
    /*
     * With the multi interface, we won't get status until the transaction is done.
     * Here we assume that if someone wants status, they want the request completed, so
     * complete it.
     */
    while (mInProgress && mHttpClient->ContinueRequest() && (mResponseCode <= 0));
    
    /*
     * If the response code is still zero, the socket may have been closed or there is some error.
     * If that is the case, masquerade as a 503 (service unavailble) error.
     */
    if (!mInProgress && mResponseCode == 0)
    {
        return 503;
    }
    return mResponseCode;
}

void CurlHttpResponse::SetResponseCode(int responseCode)
{
    mResponseCode = responseCode;
}

string CurlHttpResponse::GetHeader(string name) const
{
    StringMap::const_iterator i = mHeaders.find(ba::to_lower_copy(name));
    if (i != mHeaders.end())
    {
        return i->second;
    }
    else
    {
        return "";
    }
}

const StringMap& CurlHttpResponse::GetHeaders() const
{
    return mHeaders;
}

void CurlHttpResponse::SetHeader(string name, string value)
{
    mHeaders[ba::to_lower_copy(name)] = value;
}

istreamPtr CurlHttpResponse::GetInputStream() const
{
    return mStream;
}

void CurlHttpResponse::SetHttpClient(CurlHttpClient* httpClient)
{
    mHttpClient = httpClient;
}

void CurlHttpResponse::SetHttpRequest(RetsHttpRequest* httpRequest)
{
    mHttpRequest = httpRequest;
}

void CurlHttpResponse::SetInProgress(bool inProgress)
{
    mInProgress = inProgress;
}

string CurlHttpResponse::GetAdditionalErrorText()
{
    return mErrorText;
}

void CurlHttpResponse::SetAdditionalErrorText(string errorText)
{
    mErrorText = errorText;
}

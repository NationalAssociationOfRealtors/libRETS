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
#include "librets.h"
#include "librets/CurlHttpClient.h"
#include "librets/XmlMetadataParser.h"
#include "librets/MetadataByLevelCollector.h"
#include "librets/SearchRequest.h"
#include "librets/GetObjectRequest.h"
#include "librets/GetObjectResponse.h"
#include "librets/str_stream.h"
#include "librets/ExceptionErrorHandler.h"

using namespace librets;
using namespace librets::util;
using std::ostringstream;
using std::string;

#define CLASS RetsSession

const char * CLASS::DEFAULT_USER_AGENT = "librets/" LIBRETS_VERSION;
const RetsVersion CLASS::DEFAULT_RETS_VERSION = RETS_1_5;
const char * CLASS::RETS_VERSION_HEADER = "RETS-Version";
const char * CLASS::RETS_1_0_STRING = "RETS/1.0";
const char * CLASS::RETS_1_5_STRING = "RETS/1.5";

CLASS::CLASS(string login_url)
{
    mLoginUrl = login_url;
    mHttpMethod = RetsHttpRequest::POST;
    mHttpClient.reset(new CurlHttpClient());
    mHttpClient->SetUserAgent(DEFAULT_USER_AGENT);
    mRetsVersion = DEFAULT_RETS_VERSION;
    mErrorHandler = ExceptionErrorHandler::GetInstance();
}

void CLASS::AssertSuccessfulResponse(RetsHttpResponsePtr response,
                                     string url)
{
    int responseCode = response->GetResponseCode();
    if (responseCode != 200)
    {
        ostringstream message;
        message << "Could not get URL [ " << url << ": "
                << responseCode;
        throw RetsException(message.str());
    }
}

bool CLASS::Login(string user_name, string passwd)
{
    mHttpClient->SetDefaultHeader(RETS_VERSION_HEADER,
                                  RetsVersionToString(mRetsVersion));
    RetsHttpRequestPtr request(new RetsHttpRequest());
    request->SetUrl(mLoginUrl);
    mHttpClient->SetUserCredentials(user_name, passwd);
    RetsHttpResponsePtr httpResponse(mHttpClient->DoRequest(request));
    if (httpResponse->GetResponseCode() == 401)
    {
        httpResponse = mHttpClient->DoRequest(request);
        if (httpResponse->GetResponseCode() == 401)
        {
            return false;
        }
    }

    AssertSuccessfulResponse(httpResponse, mLoginUrl);
    mDetectedRetsVersion =
        RetsVersionFromString(httpResponse->GetHeader(RETS_VERSION_HEADER));
    mHttpClient->SetDefaultHeader(RETS_VERSION_HEADER,
                                  RetsVersionToString(mDetectedRetsVersion));
    
    LoginResponsePtr response(new LoginResponse);
    response->Parse(httpResponse->GetInputStream(), mDetectedRetsVersion);
    mCapabilityUrls = response->GetCapabilityUrls(mLoginUrl);

    RetrieveAction();

    return true;
}

void CLASS::RetrieveAction()
{
    string actionUrl = mCapabilityUrls->GetActionUrl();
    if (actionUrl == "")
    {
        mAction = "";
        return;
    }

    RetsHttpRequestPtr request(new RetsHttpRequest());
    request->SetUrl(actionUrl);
    RetsHttpResponsePtr httpResponse(mHttpClient->DoRequest(request));
    AssertSuccessfulResponse(httpResponse, actionUrl);
    mAction = readIntoString(*httpResponse->GetInputStream());
}

string CLASS::GetAction()
{
    return mAction;
}

RetsMetadataPtr CLASS::GetMetadata()
{
    if (!mMetadata)
    {
        RetrieveMetadata();
    }
    return mMetadata;
}

void CLASS::RetrieveMetadata()
{
    string getMetadataUrl = mCapabilityUrls->GetGetMetadataUrl();
    RetsHttpRequestPtr request(new RetsHttpRequest());
    request->SetUrl(getMetadataUrl);
    request->SetMethod(mHttpMethod);
    request->SetQueryParameter("Type", "METADATA-SYSTEM");
    request->SetQueryParameter("ID", "*");
    request->SetQueryParameter("Format", "COMPACT");
    RetsHttpResponsePtr httpResponse(mHttpClient->DoRequest(request));
    AssertSuccessfulResponse(httpResponse, getMetadataUrl);
    
    MetadataByLevelCollectorPtr collector(new MetadataByLevelCollector());
    XmlMetadataParserPtr parser(
        new XmlMetadataParser(collector, mErrorHandler));
    parser->Parse(httpResponse->GetInputStream());

    mMetadata.reset(new RetsMetadata(collector));
}

SearchRequestPtr CLASS::CreateSearchRequest(string searchType, 
                                            string searchClass,
                                            string query)
{
    SearchRequestPtr searchRequest(new SearchRequest(searchType, searchClass,
                                                     query));
    if (mDetectedRetsVersion == RETS_1_0)
    {
        searchRequest->SetQueryType(SearchRequest::DMQL);
    }
    else
    {
        searchRequest->SetQueryType(SearchRequest::DMQL2);
    }
    return searchRequest;
}

SearchResultSetPtr CLASS::Search(SearchRequestPtr request)
{
    string searchUrl = mCapabilityUrls->GetSearchUrl();
    request->SetUrl(searchUrl);
    request->SetMethod(mHttpMethod);
    RetsHttpResponsePtr httpResponse = mHttpClient->DoRequest(request);
    AssertSuccessfulResponse(httpResponse, searchUrl);
    
    SearchResultSetPtr resultSet(new SearchResultSet());
    resultSet->Parse(httpResponse->GetInputStream());
    return resultSet;
}

GetObjectResponsePtr CLASS::GetObject(GetObjectRequestPtr request)
{
    RetsHttpRequestPtr httpRequest = request->CreateHttpRequest();
    string getObjectUrl = mCapabilityUrls->GetGetObjectUrl();
    httpRequest->SetUrl(getObjectUrl);
    httpRequest->SetMethod(mHttpMethod);
    RetsHttpResponsePtr httpResponse = mHttpClient->DoRequest(httpRequest);
    AssertSuccessfulResponse(httpResponse, getObjectUrl);
    
    GetObjectResponsePtr response(new GetObjectResponse());
    response->Parse(httpResponse);
    return response;
}

LogoutResponsePtr CLASS::Logout()
{
    LogoutResponsePtr logoutResponse;
    string logoutUrl = mCapabilityUrls->GetLogoutUrl();
    if (logoutUrl == "")
    {
        return logoutResponse;
    }
    
    RetsHttpRequestPtr request(new RetsHttpRequest());
    request->SetUrl(logoutUrl);
    RetsHttpResponsePtr httpResponse(mHttpClient->DoRequest(request));
    AssertSuccessfulResponse(httpResponse, logoutUrl);

    logoutResponse.reset(new LogoutResponse());
    logoutResponse->Parse(httpResponse->GetInputStream(), mDetectedRetsVersion);
    return logoutResponse;
}

void CLASS::SetUserAgent(string userAgent)
{
    mHttpClient->SetUserAgent(userAgent);
}

void CLASS::UseHttpGet(bool useHttpGet)
{
    if (useHttpGet)
    {
        mHttpMethod = RetsHttpRequest::GET;
    }
    else
    {
        mHttpMethod = RetsHttpRequest::POST;
    }
}

void CLASS::SetHttpLogger(RetsHttpLogger * logger)
{
    mHttpClient->SetLogger(logger);
}

RetsVersion CLASS::GetRetsVersion() const
{
    return mRetsVersion;
}

void CLASS::SetRetsVersion(RetsVersion retsVersion)
{
    mRetsVersion = retsVersion;
}

RetsVersion CLASS::GetDetectedRetsVersion() const
{
    return mDetectedRetsVersion;
}

string CLASS::RetsVersionToString(RetsVersion retsVersion)
{
    if (retsVersion == RETS_1_0)
    {
        return RETS_1_0_STRING;
    }
    else if (retsVersion == RETS_1_5)
    {
        return RETS_1_5_STRING;
    }
    else
    {
        throw RetsException(str_stream() << "Invalid RetsVersion: "
                            << retsVersion);
    }
}

RetsVersion CLASS::RetsVersionFromString(string versionString)
{
    if (versionString == RETS_1_0_STRING)
    {
        return RETS_1_0;
    }
    else if (versionString == RETS_1_5_STRING)
    {
        return RETS_1_5;
    }
    else if (versionString.empty())
    {
        return RETS_1_0;
    }
    else
    {
        throw RetsException(str_stream() << "Invalid RETS version string: "
                            << versionString);
    }
}

void CLASS::SetErrorHandler(RetsErrorHandler * errorHandler)
{
    mErrorHandler = errorHandler;
}

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
#include "librets/DefaultMetadataCollector.h"
#include "librets/IncrementalMetadataFinder.h"
#include "librets/SearchRequest.h"
#include "librets/GetObjectRequest.h"
#include "librets/GetObjectResponse.h"
#include "librets/str_stream.h"
#include "librets/ExceptionErrorHandler.h"

using namespace librets;
using namespace librets::util;
using std::ostringstream;
using std::string;

typedef RetsSession CLASS;

const char * CLASS::DEFAULT_USER_AGENT = "librets/" LIBRETS_VERSION;
const RetsVersion CLASS::DEFAULT_RETS_VERSION = RETS_1_5;
const char * CLASS::RETS_VERSION_HEADER = "RETS-Version";
const char * CLASS::RETS_1_0_STRING = "RETS/1.0";
const char * CLASS::RETS_1_5_STRING = "RETS/1.5";

CLASS::RetsSession(string login_url)
{
    mLoginUrl = login_url;
    mHttpMethod = RetsHttpRequest::POST;
    mHttpClient.reset(new CurlHttpClient());
    mHttpClient->SetUserAgent(DEFAULT_USER_AGENT);
    mRetsVersion = DEFAULT_RETS_VERSION;
    mErrorHandler = ExceptionErrorHandler::GetInstance();
    mIncrementalMetadata = true;
    mUserAgentAuthType = USER_AGENT_AUTH_NONE;
}

RetsHttpResponsePtr CLASS::DoRequest(RetsHttpRequest * request)
{
    if (mUserAgentAuthType != USER_AGENT_AUTH_NONE)
    {
        mUserAgentAuthCalculator.SetRequestId("");
        mUserAgentAuthCalculator.SetSessionId("");
        mUserAgentAuthCalculator.SetVersionInfo(
            mHttpClient->GetDefaultHeader(RETS_VERSION_HEADER));
        string headerValue = "Digest " + mUserAgentAuthCalculator.AuthorizationValue();
        request->SetHeader("RETS-UA-Authorization", headerValue);
    }
    return mHttpClient->DoRequest(request);
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
    RetsHttpRequest request;
    request.SetUrl(mLoginUrl);
    mHttpClient->SetUserCredentials(user_name, passwd);
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    if (httpResponse->GetResponseCode() == 401)
    {
        httpResponse = DoRequest(&request);
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
    
    LoginResponse response;
    response.Parse(httpResponse->GetInputStream(), mDetectedRetsVersion);
    mCapabilityUrls = response.GetCapabilityUrls(mLoginUrl);

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

    RetsHttpRequest request;
    request.SetUrl(actionUrl);
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    AssertSuccessfulResponse(httpResponse, actionUrl);
    mAction = readIntoString(*httpResponse->GetInputStream());
}

string CLASS::GetAction()
{
    return mAction;
}

RetsMetadata * CLASS::GetMetadata()
{
    if (!mMetadata)
    {
        InitializeMetadata();
    }
    return mMetadata.get();
}

bool CLASS::IsIncrementalMetadata() const
{
    return mIncrementalMetadata;
}

void CLASS::SetIncrementalMetadata(bool incrementalMetadata)
{
    mIncrementalMetadata = incrementalMetadata;
}

void CLASS::SetCollector(MetadataElementCollectorPtr collector)
{
    mLoaderCollector = collector;
}

std::string CLASS::MetadataTypeToString(MetadataElement::Type type)
{
    if (type == MetadataElement::SYSTEM)
        return "METADATA-SYSTEM";
    else if (type == MetadataElement::CLASS)
        return "METADATA-CLASS";
    else if (type == MetadataElement::RESOURCE)
        return "METADATA-RESOURCE";
    else if (type == MetadataElement::TABLE)
        return "METADATA-TABLE";
    else if (type == MetadataElement::LOOKUP)
        return "METADATA-LOOKUP";
    else if (type == MetadataElement::LOOKUP_TYPE)
        return "METADATA-LOOKUP_TYPE";
    else
    {
        throw RetsException(str_stream() << "Invalid metadata type: "
                            << type);
    }
}

void CLASS::LoadMetadata(MetadataElement::Type type,
                         std::string level)
{
    string getMetadataUrl = mCapabilityUrls->GetGetMetadataUrl();
    RetsHttpRequest request;
    request.SetUrl(getMetadataUrl);
    request.SetMethod(mHttpMethod);
    request.SetQueryParameter("Type", MetadataTypeToString(type));
    request.SetQueryParameter("ID", level.empty() ? "0" : level);
    request.SetQueryParameter("Format", "COMPACT");
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    AssertSuccessfulResponse(httpResponse, getMetadataUrl);
    
    XmlMetadataParserPtr parser(new XmlMetadataParser(mLoaderCollector,
                                                      mErrorHandler));
    parser->Parse(httpResponse->GetInputStream());
}


void CLASS::InitializeMetadata()
{
    if (mIncrementalMetadata)
    {
        IncrementalMetadataFinderPtr
            incrementalFinder(new IncrementalMetadataFinder(this));
        mMetadata.reset(new RetsMetadata(incrementalFinder));
    }
    else
    {
        RetrieveFullMetadata();
    }
}

void CLASS::RetrieveFullMetadata()
{
    string getMetadataUrl = mCapabilityUrls->GetGetMetadataUrl();
    RetsHttpRequest request;
    request.SetUrl(getMetadataUrl);
    request.SetMethod(mHttpMethod);
    request.SetQueryParameter("Type", "METADATA-SYSTEM");
    request.SetQueryParameter("ID", "*");
    request.SetQueryParameter("Format", "COMPACT");
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    AssertSuccessfulResponse(httpResponse, getMetadataUrl);
    
    DefaultMetadataCollectorPtr collector(new DefaultMetadataCollector());
    XmlMetadataParserPtr parser(
        new XmlMetadataParser(collector, mErrorHandler));
    parser->Parse(httpResponse->GetInputStream());

    mMetadata.reset(new RetsMetadata(collector));
}

SearchRequestAPtr CLASS::CreateSearchRequest(string searchType, 
                                             string searchClass,
                                             string query)
{
    std::auto_ptr<SearchRequest> searchRequest(
        new SearchRequest(searchType, searchClass, query));
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

SearchResultSetAPtr CLASS::Search(SearchRequest * request)
{
    string searchUrl = mCapabilityUrls->GetSearchUrl();
    request->SetUrl(searchUrl);
    request->SetMethod(mHttpMethod);
    RetsHttpResponsePtr httpResponse = DoRequest(request);
    AssertSuccessfulResponse(httpResponse, searchUrl);
    
    SearchResultSetAPtr resultSet(new SearchResultSet());
    resultSet->Parse(httpResponse->GetInputStream());
    return resultSet;
}

GetObjectResponseAPtr CLASS::GetObject(GetObjectRequest * request)
{
    RetsHttpRequestPtr httpRequest = request->CreateHttpRequest();
    string getObjectUrl = mCapabilityUrls->GetGetObjectUrl();
    httpRequest->SetUrl(getObjectUrl);
    httpRequest->SetMethod(mHttpMethod);
    RetsHttpResponsePtr httpResponse = DoRequest(httpRequest.get());
    AssertSuccessfulResponse(httpResponse, getObjectUrl);
    
    GetObjectResponseAPtr response(new GetObjectResponse());
    if (request->HasDefaultObjectKeyAndId())
    {
        response->SetDefaultObjectKeyAndId(request->GetDefaultObjectKey(),
                                           request->GetDefaultObjectId());
    }
    response->Parse(httpResponse);
    return response;
}

LogoutResponseAPtr CLASS::Logout()
{
    LogoutResponseAPtr logoutResponse;
    string logoutUrl = mCapabilityUrls->GetLogoutUrl();
    if (logoutUrl == "")
    {
        return logoutResponse;
    }
    
    RetsHttpRequest request;
    request.SetUrl(logoutUrl);
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    AssertSuccessfulResponse(httpResponse, logoutUrl);

    logoutResponse.reset(new LogoutResponse());
    logoutResponse->Parse(httpResponse->GetInputStream(), mDetectedRetsVersion);
    return logoutResponse;
}

void CLASS::SetUserAgent(string userAgent)
{
    mHttpClient->SetUserAgent(userAgent);
    mUserAgentAuthCalculator.SetUserAgent(userAgent);
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

void CLASS::SetUserAgentAuthType(UserAgentAuthType type)
{
    mUserAgentAuthType = type;
}

UserAgentAuthType CLASS::GetUserAgentAuthType() const
{
    return mUserAgentAuthType;
}

void CLASS::SetUserAgentPassword(std::string userAgentPassword)
{
    mUserAgentAuthCalculator.SetUserAgentPassword(userAgentPassword);
}


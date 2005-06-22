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

using namespace librets;
using namespace librets::util;
using std::ostringstream;
using std::string;

#define CLASS RetsSession

CLASS::CLASS(string login_url)
{
    mLoginUrl = login_url;
    mHttpClient.reset(new CurlHttpClient());
    mHttpClient->AddDefaultHeader("RETS-Version", "RETS/1.5");
    mHttpClient->AddDefaultHeader("User-Agent", "librets/0.1");
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
    LoginResponsePtr response(new LoginResponse);
    response->Parse(httpResponse->GetInputStream());
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
    request->SetQueryParameter("Type", "METADATA-SYSTEM");
    request->SetQueryParameter("ID", "*");
    request->SetQueryParameter("Format", "COMPACT");
    RetsHttpResponsePtr httpResponse(mHttpClient->DoRequest(request));
    AssertSuccessfulResponse(httpResponse, getMetadataUrl);
    
    MetadataByLevelCollectorPtr collector(new MetadataByLevelCollector());
    XmlMetadataParserPtr parser(new XmlMetadataParser(collector));
    parser->Parse(httpResponse->GetInputStream());

    mMetadata.reset(new RetsMetadata(collector));
}

SearchResultSetPtr CLASS::Search(SearchRequestPtr request)
{
    string searchUrl = mCapabilityUrls->GetSearchUrl();
    request->SetUrl(searchUrl);
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
    logoutResponse->Parse(httpResponse->GetInputStream());
    return logoutResponse;
}

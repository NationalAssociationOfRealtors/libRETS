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
#include <string>
#include "librets/CapabilityUrls.h"
#include "librets/Uri.h"

using namespace librets;
using std::string;
using std::ostream;
using std::endl;

#define CLASS CapabilityUrls

CLASS::CLASS(string baseUrl)
{
    mBaseUrl = baseUrl;
}

CLASS::~CLASS()
{
}

string CLASS::ResolveUrl(string url)
{
    if (url == "")
    {
        return "";
    }
    return Uri::Resolve(url, mBaseUrl);
}

void CLASS::SetActionUrl(string actionUrl)
{
    mActionUrl = ResolveUrl(actionUrl);
}

string CLASS::GetActionUrl() const
{
    return mActionUrl;
}

void CLASS::SetChangePasswordUrl(string changePasswordUrl)
{
    mChangePasswordUrl = ResolveUrl(changePasswordUrl);
}

string CLASS::GetChangePasswordUrl() const
{
    return mChangePasswordUrl;
}

void CLASS::SetGetObjectUrl(string getObjectUrl)
{
    mGetObjectUrl = ResolveUrl(getObjectUrl);
}

string CLASS::GetGetObjectUrl() const
{
    return mGetObjectUrl;
}

void CLASS::SetLoginUrl(string loginUrl)
{
    mLoginUrl = loginUrl;
}

string CLASS::GetLoginUrl() const
{
    return mLoginUrl;
}

void CLASS::SetLoginCompleteUrl(string loginCompleteUrl)
{
    mLoginCompleteUrl = ResolveUrl(loginCompleteUrl);
}

string CLASS::GetLoginCompleteUrl() const
{
    return mLoginCompleteUrl;
}

void CLASS::SetLogoutUrl(string logoutUrl)
{
    mLogoutUrl = ResolveUrl(logoutUrl);
}

string CLASS::GetLogoutUrl() const
{
    return mLogoutUrl;
}

void CLASS::SetSearchUrl(string searchUrl)
{
    mSearchUrl = ResolveUrl(searchUrl);
}

string CLASS::GetSearchUrl() const
{
    return mSearchUrl;
}

void CLASS::SetGetMetadataUrl(string getMetadataUrl)
{
    mGetMetadataUrl = ResolveUrl(getMetadataUrl);
}

string CLASS::GetGetMetadataUrl() const
{
    return mGetMetadataUrl;
}

void CLASS::SetServerInformationUrl(string serverInformationUrl)
{
    mServerInformationUrl = ResolveUrl(serverInformationUrl);
}

string CLASS::GetServerInformationUrl() const
{
    return mServerInformationUrl;
}

void CLASS::SetUpdateUrl(string updateUrl)
{
    mUpdateUrl = ResolveUrl(updateUrl);
}

string CLASS::GetUpdateUrl() const
{
    return mUpdateUrl;
}

void CLASS::SetPayloadListUrl(string payloadListUrl)
{
    mPayloadListUrl = ResolveUrl(payloadListUrl);
}

string CLASS::GetPayloadListUrl() const
{
    return mPayloadListUrl;
}

ostream & CLASS::Print(ostream & outputStream) const
{
    outputStream << "Capability URLS [\n";
    outputStream << "Action = " << mActionUrl << endl;
    outputStream << "ChangePassword = " << mChangePasswordUrl << endl;
    outputStream << "GetObject = " << mGetObjectUrl << endl;
    outputStream << "Login = " << mLoginUrl << endl;
    outputStream << "LoginComplete = " << mLoginCompleteUrl << endl;
    outputStream << "Logout = " << mLogoutUrl << endl;
    outputStream << "Search = " << mSearchUrl << endl;
    outputStream << "GetMetadata = " << mGetMetadataUrl << endl;
    outputStream << "ServerInformation = " << mServerInformationUrl << endl;
    outputStream << "Update = " << mUpdateUrl << endl;
    outputStream << "GetPayloadList = " << mPayloadListUrl << endl;
    return outputStream << "]";
}

bool CLASS::Equals(const RetsObject * object) const
{
    const CapabilityUrls * rhs =
        dynamic_cast<const CapabilityUrls *> (object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mActionUrl == rhs->mActionUrl);
    equals &= (mChangePasswordUrl == rhs->mChangePasswordUrl);
    equals &= (mGetObjectUrl == rhs->mGetObjectUrl);
    equals &= (mLoginUrl == rhs->mLoginUrl);
    equals &= (mLoginCompleteUrl == rhs->mLoginCompleteUrl);
    equals &= (mLogoutUrl == rhs->mLogoutUrl);
    equals &= (mSearchUrl == rhs->mSearchUrl);
    equals &= (mGetMetadataUrl == rhs->mGetMetadataUrl);
    equals &= (mServerInformationUrl == rhs->mServerInformationUrl);
    equals &= (mUpdateUrl == rhs->mUpdateUrl);
    equals &= (mPayloadListUrl == rhs->mPayloadListUrl);
return equals;
}

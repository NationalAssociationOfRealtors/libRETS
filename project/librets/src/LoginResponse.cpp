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
#include <vector>
#include "librets/LoginResponse.h"
#include "librets/CapabilityUrls.h"

using namespace librets;
using std::string;
using std::vector;
namespace b = boost;

#define CLASS LoginResponse

string CLASS::GetMemberName() const
{
    return GetValue("MemberName");
}

string CLASS::GetUserInfo() const
{
    return GetValue("User");
}

string CLASS::GetBroker() const
{
    return GetValue("Broker");
}

string CLASS::GetMetadataVersion() const
{
    return GetValue("MetadataVersion");
}

string CLASS::GetMetadataTimestamp() const
{
    return GetValue("MetadataTimestamp");
}

string CLASS::GetMinMetadataTimestamp() const
{
    return GetValue("MinMetadataTimestamp");
}

string CLASS::GetOfficeList() const
{
    return GetValue("OfficeLIst");
}

string CLASS::GetBalance() const
{
    return GetValue("Balance");
}

string CLASS::GetTimeout() const
{
    return GetValue("TimeoutSeconds");
}

string CLASS::GetPasswordExpire() const
{
    return GetValue("Expr");
}

string CLASS::GetActionUrl() const
{
    return GetValue("Action");
}

string CLASS::GetChangePasswordUrl() const
{
    return GetValue("ChangePassword");
}

string CLASS::GetGetObjectUrl() const
{
    return GetValue("GetObject");
}

string CLASS::GetLoginUrl() const
{
    return GetValue("Login");
}

string CLASS::GetLoginCompleteUrl() const
{
    return GetValue("LoginComplete");
}

string CLASS::GetLogoutUrl() const
{
    return GetValue("Logout");
}

string CLASS::GetSearchUrl() const
{
    return GetValue("Search");
}

string CLASS::GetGetMetadataUrl() const
{
    return GetValue("GetMetadata");
}

string CLASS::GetServerInformationUrl() const
{
    return GetValue("ServerInformation");
}

string CLASS::GetUpdateUrl() const
{
    return GetValue("Update");
}

CapabilityUrlsPtr CLASS::GetCapabilityUrls(string baseUrl) const
{
    CapabilityUrlsPtr urls(new CapabilityUrls(baseUrl));
    urls->SetActionUrl(GetActionUrl());
    urls->SetChangePasswordUrl(GetChangePasswordUrl());
    urls->SetGetObjectUrl(GetGetObjectUrl());
    urls->SetLoginUrl(GetLoginUrl());
    urls->SetLoginCompleteUrl(GetLoginCompleteUrl());
    urls->SetLogoutUrl(GetLogoutUrl());
    urls->SetSearchUrl(GetSearchUrl());
    urls->SetGetMetadataUrl(GetGetMetadataUrl());
    urls->SetServerInformationUrl(GetServerInformationUrl());
    urls->SetUpdateUrl(GetUpdateUrl());
    return urls;
}

RetsXmlTextEventPtr CLASS::GetBodyEvent(RetsXmlEventListPtr eventList,
                                        RetsVersion retsVersion)
{
    if (retsVersion != RETS_1_0)
    {
        return GetBodyEventFromStandardResponse(eventList);
    }
    else
    {
        return GetBodyEventFromResponseWithNoRetsResponse(eventList);
    }
}

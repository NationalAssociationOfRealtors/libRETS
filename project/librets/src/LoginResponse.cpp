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
    string name = mSessionInfo.GetValue("MemberName");
    if (!name.empty())
    {
        return name;
    }
    return GetValue("MemberName");
}

string CLASS::GetUserInfo() const
{
    string name = mSessionInfo.GetValue("UserID");
    if (!name.empty())
    {
        return name;
    }        
    return GetValue("User");
}

string CLASS::GetBroker() const
{
    string name = mSessionInfo.GetValue("BrokerCode");
    if (!name.empty())
    {
        return name;
    }    
    return GetValue("Broker");
}

string CLASS::GetMetadataVersion() const
{
    string name = mSessionInfo.GetValue("MetadataVersion");
    if (!name.empty())
    {
        return name;
    }        
    return GetValue("MetadataVersion");
}

string CLASS::GetMetadataTimestamp() const
{
    string name = mSessionInfo.GetValue("MetadataTimestamp");
    if (!name.empty())
    {
        return name;
    }        
    return GetValue("MetadataTimestamp");
}

string CLASS::GetMinMetadataTimestamp() const
{
    string name = mSessionInfo.GetValue("MinMetadataTimestamp");
    if (!name.empty())
    {
        return name;
    }        
    return GetValue("MinMetadataTimestamp");
}

string CLASS::GetOfficeList() const
{
    string name = mSessionInfo.GetValue("OfficeList");
    if (!name.empty())
    {
        return name;
    }            
    return GetValue("OfficeList");
}

string CLASS::GetBalance() const
{
    string name = mSessionInfo.GetValue("Balance");
    if (!name.empty())
    {
        return name;
    }            
    return GetValue("Balance");
}

string CLASS::GetTimeout() const
{
    string name = mSessionInfo.GetValue("TimeoutSeconds");
    if (!name.empty())
    {
        return name;
    }            
    return GetValue("TimeoutSeconds");
}

string CLASS::GetPasswordExpire() const
{
    string name = mSessionInfo.GetValue("PasswordExpiration");
    if (!name.empty())
    {
        return name;
    }            
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

string CLASS::GetPayloadListUrl() const
{
    return GetValue("GetPayloadList");
}

string CLASS::GetUserID() const
{
    return mSessionInfo.GetValue("USERID");
}

string CLASS::GetUserClass() const
{
    return mSessionInfo.GetValue("USERCLASS");
}

string CLASS::GetUserLevel() const
{
    return mSessionInfo.GetValue("USERLEVEL");
}

string CLASS::GetAgentCode() const
{
    return mSessionInfo.GetValue("AGENTCODE");
}

string CLASS::GetBrokerCode() const
{
    return mSessionInfo.GetValue("BROKERCODE");
}

string CLASS::GetBrokerBranch() const
{
    return mSessionInfo.GetValue("BROKERBRANCH");
}

string CLASS::GetMetadataID() const
{
    return mSessionInfo.GetValue("MetadataID");
}

string CLASS::GetWarnPasswordExpirationDays() const
{
    return GetValue("WarnPasswordExpirationDays");
}

string CLASS::GetStandardNamesVersion() const
{
    return mSessionInfo.GetValue("StandardNameVersion");
}

string CLASS::GetVendorName() const
{
    return mSessionInfo.GetValue("VendorName");
}

string CLASS::GetServerProductName() const
{
    return mSessionInfo.GetValue("ServerProductName");
}

string CLASS::GetServerProductVersion() const
{
    return mSessionInfo.GetValue("ServerProductVersion");
}

string CLASS::GetOperatorName() const
{
    return mSessionInfo.GetValue("OperatorName");
}

string CLASS::GetRoleName() const
{
    return mSessionInfo.GetValue("RoleName");
}

string CLASS::GetSupportContactInformation() const
{
    return mSessionInfo.GetValue("SupportContactInformation");
}

string CLASS::GetSessionInformationTokens() const
{
    return mSessionInfo.GetSessionInformationTokens();
}

CapabilityUrlsAPtr CLASS::CreateCapabilityUrls(string baseUrl) const
{
    CapabilityUrlsAPtr urls(new CapabilityUrls(baseUrl));
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
    urls->SetPayloadListUrl(GetPayloadListUrl());
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

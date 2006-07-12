/*
 * Copyright (C) 2006 National Association of REALTORS(R)
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

#include <boostext/md5.hpp>
#include "librets/UserAgentAuthCalculator.h"

using namespace librets;
using std::string;
namespace be = boostext;

#define CLASS UserAgentAuthCalculator

void CLASS::SetUserAgent(string userAgent)
{
    mUserAgent = userAgent;
}

void CLASS::SetUserAgentPassword(string userAgentPassword)
{
    mUserAgentPassword = userAgentPassword;
}

void CLASS::SetRequestId(string requestId)
{
    mRequestId = requestId;
}

void CLASS::SetSessionId(string sessionId)
{
    mSessionId = sessionId;
}

void CLASS::SetVersionInfo(string versionInfo)
{
    mVersionInfo = versionInfo;
}

bool CLASS::HasAuthorizationValue() const
{
    return (!mUserAgentPassword.empty());
}

string CLASS::AuthorizationValue() const
{
    if (!HasAuthorizationValue())
        return "";
    
    string product = mUserAgent;
    string::size_type slashPosition = product.find_first_of('/');
    if (slashPosition != string::npos)
    {
        product.resize(slashPosition);
    }
    
    string a1 = product + ":" + mUserAgentPassword;
    string a1_digest = be::md5(a1).digest().hex_str_value();
    string response = a1_digest + ":" + mRequestId + ":" + mSessionId + ":" +
        mVersionInfo;
    string response_digest = be::md5(response).digest().hex_str_value();
    return response_digest;
}

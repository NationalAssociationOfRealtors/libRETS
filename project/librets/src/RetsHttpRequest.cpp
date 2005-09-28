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
#include <boost/lexical_cast.hpp>                                               
#include "librets/RetsHttpRequestImpl.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostringstream;
using boost::lexical_cast; 

#define CLASS RetsHttpRequestImpl

CLASS::CLASS()
    : mQueryParameters(), mQueryParametersChanged(false)
{
    mMethod = GET;
}

void CLASS::SetUrl(string url)
{
    mUrl = url;
}

string CLASS::GetUrl() const
{
    return mUrl;
}

void CLASS::SetMethod(Method method)
{
    mMethod = method;
}

CLASS::Method CLASS::GetMethod() const
{
    return mMethod;
}

void CLASS::SetHeader(string name, string value)
{
    mHeaders[name] = value;
}

void CLASS::ClearHeader(string name)
{
    mHeaders.erase(name);
}

StringMap CLASS::GetHeaderMap() const
{
    return mHeaders;
}

void CLASS::SetQueryParameter(string name, int value)
{
    SetQueryParameter(name, lexical_cast<string>(value));
}

void CLASS::SetQueryParameter(string name, string value)
{
    if (!value.empty())
    {
        mQueryParameters[name] = urlEncode(value);
    }
    else
    {
        mQueryParameters.erase(name);
    }
    mQueryParametersChanged = true;
}

void CLASS::GenerateQueryString() const
{
    if (!mQueryParametersChanged)
        return;
    
    ostringstream queryString;
    string separator = "";
    StringMap::const_iterator i;
    for (i = mQueryParameters.begin(); i != mQueryParameters.end(); i++)
    {
        const string & name = i->first;
        const string & value = i-> second;
        queryString << separator << name << "=" << value;
        separator = "&";
    }
    mQueryString = queryString.str();
    mQueryParametersChanged = false;
}

std::string CLASS::GetQueryString() const
{
    GenerateQueryString();
    return mQueryString;
}

#undef CLASS
#define CLASS RetsHttpRequest
#include <iostream>

CLASS::CLASS()
    : mImpl(new RetsHttpRequestImpl())
{
}

CLASS::CLASS(RetsHttpRequestImplPtr impl)
    : mImpl(impl)
{
}

RetsHttpRequestImplPtr CLASS::GetRetsHttpRequestImpl() const
{
    return mImpl;
}

CLASS::Method CLASS::GetMethod() const
{
    return mImpl->GetMethod();
}

void CLASS::SetMethod(CLASS::Method method)
{
    mImpl->SetMethod(method);
}

string CLASS::GetUrl() const
{
    return mImpl->GetUrl();
}

void CLASS::SetUrl(string url)
{
    mImpl->SetUrl(url);
}

void CLASS::SetHeader(string name, string value)
{
    mImpl->SetHeader(name, value);
}

void CLASS::ClearHeader(string name)
{
    mImpl->ClearHeader(name);
}

StringMap CLASS::GetHeaderMap() const
{
    return mImpl->GetHeaderMap();
}

void CLASS::SetQueryParameter(string name, string value)
{
    mImpl->SetQueryParameter(name, value);
}

void CLASS::SetQueryParameter(string name, int value)
{
    mImpl->SetQueryParameter(name, value);
}

string CLASS::GetQueryString() const
{
    return mImpl->GetQueryString();
}


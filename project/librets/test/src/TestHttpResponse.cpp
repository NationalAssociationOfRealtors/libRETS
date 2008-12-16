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

#include <boost/algorithm/string.hpp>
#include "TestHttpResponse.h"
#include "testUtil.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::getline;
using std::ios_base;
namespace ba = boost::algorithm;

TestHttpResponse::TestHttpResponse(string resourceName)
{
    mInputStream = getResource(resourceName, ios_base::binary);
    *mInputStream >> mResponseCode;
    string line;
    // Get blank line after response code
    getline(*mInputStream, line);
    while (true)
    {
        getline(*mInputStream, line);
        if (line.empty())
        {
            break;
        }
        string name;
        string value;
        if (splitField(line, ":", name, value))
        {
            mHeaders[ba::to_lower_copy(name)] = ba::trim_copy(value);
        }
    }
}

TestHttpResponse::~TestHttpResponse()
{
}

void TestHttpResponse::SetResponseCode(int responseCode)
{
    mResponseCode = responseCode;
}

int TestHttpResponse::GetResponseCode() const
{
    return mResponseCode;
}

string TestHttpResponse::GetHeader(string name) const
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

istreamPtr TestHttpResponse::GetInputStream() const
{
    return mInputStream;
}

string TestHttpResponse::GetAdditionalErrorText()
{
    return "";
}

void TestHttpResponse::SetAdditionalErrorText(string errorText)
{
}

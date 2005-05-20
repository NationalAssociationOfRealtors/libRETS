/*
 *  TestHttpResponse.cpp
 *  librets
 *
 *  Created by Dave Dribin on 4/5/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
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
            mHeaders[name] = ba::trim_copy(value);
        }
    }
}

TestHttpResponse::~TestHttpResponse()
{
}

int TestHttpResponse::GetResponseCode() const
{
    return mResponseCode;
}

string TestHttpResponse::GetHeader(string name) const
{
    StringMap::const_iterator i = mHeaders.find(name);
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

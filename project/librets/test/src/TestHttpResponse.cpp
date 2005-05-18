/*
 *  TestHttpResponse.cpp
 *  librets
 *
 *  Created by Dave Dribin on 4/5/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "TestHttpResponse.h"
#include "testUtil.h"
#include "util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::getline;

TestHttpResponse::TestHttpResponse(string resourceName)
{
    mInputStream = getResource(resourceName);
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
        StringVectorPtr header = split(line, ":", 2);
        mHeaders[header->at(0)] = trim(header->at(1));
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

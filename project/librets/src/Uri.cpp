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
#include "librets/Uri.h"
#include <sstream>

using namespace librets;
using std::string;
using std::istringstream;
using std::ostream;

#define CLASS Uri

CLASS::CLASS(string uri)
{
    init(uri);
}

void CLASS::init(const string & uri)
{
    string::size_type i1;
    string::size_type i2;
    i1 = 0;
    i2 = uri.find(":", i1);
    if (i2 != string::npos)
    {
        mIsAbsolute = true;
        mScheme = uri.substr(i1, (i2 - i1));

        // Skip over ":"
        i1 = i2 + 1;
        i2 = uri.find("//", i1);

        // Skip over "//"
        i1 = i2 + 2;
        i2 = uri.find(":", i1);
        if (i2 == string::npos)
        {
            mPort = DEFAULT_PORT;
            i2 = uri.find("/", i1);
            mHost = uri.substr(i1, (i2 - i1));
        }
        else
        {
            mHost = uri.substr(i1, (i2 - i1));
            // Skip over ":";
            i1 = i2 + 1;
            i2 = uri.find("/", i1);
            string portString = uri.substr(i1, (i2 - i1));
            istringstream s;
            s.str(portString);
            s >> mPort;
        }
        mPath = uri.substr(i2);
    }
    else
    {
        mIsAbsolute = false;
        mScheme = "";
        mHost = "";
        mPort = -1;
        mPath = uri;
    }
}

CLASS::CLASS(string uri, string baseUri)
{
    init(uri);
    Uri base(baseUri);
    if (!mIsAbsolute)
    {
        mScheme = base.mScheme;
        mHost = base.mHost;
        mPort = base.mPort;

        if (mPath.find("/") != 0)
        {
            string tempPath = mPath;
            string::size_type lastSlash = base.mPath.rfind("/");
            // Include "/" in substring
            mPath = base.mPath.substr(0, lastSlash + 1);
            mPath = mPath + tempPath;
        }
        mIsAbsolute = true;
    }
}

bool CLASS::IsAbsolute() const
{
    return mIsAbsolute;
}

string CLASS::GetScheme() const
{
    return mScheme;
}

string CLASS::GetHost() const
{
    return mHost;
}

int CLASS::GetPort() const
{
    return mPort;
}

string CLASS::GetPath() const
{
    return mPath;
}

string CLASS::Resolve(string uri, string baseUri)
{
    return Uri(uri, baseUri).ToString();
}

ostream & CLASS::Print(ostream & outputStream) const
{
    if (mIsAbsolute)
    {
        outputStream << mScheme << "://" << mHost;
        if (mPort != DEFAULT_PORT)
        {
            outputStream << ":" << mPort;
        }
        outputStream << mPath;
    }
    else
    {
        outputStream << mPath;
    }
    return outputStream;
}

bool CLASS::Equals(const RetsObject * object) const
{
    const Uri * rhs =
        dynamic_cast<const Uri *> (object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mIsAbsolute == rhs->mIsAbsolute);
    equals &= (mScheme == rhs->mScheme);
    equals &= (mHost == rhs->mHost);
    equals &= (mPort == rhs->mPort);
    equals &= (mPath == rhs->mPath);
    return equals;
}

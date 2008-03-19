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

#include <iostream>
#include "librets_sharp.h"
#include "librets/CurlStream.h"

using namespace librets;
using std::string;
using std::istream;

#define CLASS InputStreamBridge

CLASS::CLASS(istreamPtr inputStream)
    : mInputStream(inputStream)
{
    
}

int CLASS::readByte() const
{
    char byte;
    bool isCurlStream = (typeid(*mInputStream) == typeid(CurlStream));

    if (isCurlStream)
    {
	boost::dynamic_pointer_cast<CurlStream>(mInputStream)->read(&byte, sizeof(byte));
	if (boost::dynamic_pointer_cast<CurlStream>(mInputStream)->gcount() == sizeof(byte))
        {
	    return byte;
        }
	else
	{
	    return -1;
	}
    }

    mInputStream->read(&byte, sizeof(byte));
    if (mInputStream->gcount() == sizeof(byte))
    {
        return byte;
    }
    else
    {
        return -1;
    }
}
 
int CLASS::read(unsigned char buffer[], int offset, int length) const
{
    char * startOfBuffer = (char *)(buffer + offset);
    bool isCurlStream = (typeid(*mInputStream) == typeid(CurlStream));

    if (isCurlStream)
    {
	boost::dynamic_pointer_cast<CurlStream>(mInputStream)->read(startOfBuffer, length);
	return boost::dynamic_pointer_cast<CurlStream>(mInputStream)->gcount();
    }

    mInputStream->read(startOfBuffer, length);
    return mInputStream->gcount();
}

#undef CLASS
#define CLASS RetsHttpLoggerBridge

CLASS::CLASS(RetsHttpLoggerCallback loggerCallback)
    : mLoggerCallback(loggerCallback)
{
}

void CLASS::logHttpData(Type type, string data)
{
    mLoggerCallback(type, (void *) data.data(), data.length());
}

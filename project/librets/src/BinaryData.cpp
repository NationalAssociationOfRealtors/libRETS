/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
/*
 *  BinaryData.cpp
 *  librets
 *
 *  Created by Mark Klein on 12/30/08.
 *
 */

#include <iostream>
#include "librets/BinaryData.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::istream;

BinaryData::BinaryData(unsigned char buffer[], int len)
{
    mData.clear();
    mData.append((const char *)buffer, len);
    mData.append("\0");
}

int BinaryData::Size() const
{
    return mData.size();
}

string BinaryData::AsString() const
{
    return mData;
}

const char * BinaryData::AsChar() const
{
    return mData.data();
}

void BinaryData::ReadToEof(istreamPtr inputStream)
{
    mData.clear();
    readIntoString(inputStream, mData);
}

void BinaryData::Copy(unsigned char buffer[], int length) const
{
    mData.copy((char *) buffer, length);
}

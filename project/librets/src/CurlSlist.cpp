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
#include "librets/curl.h"
#include "librets/CurlSlist.h"
#include "librets/RetsException.h"

using namespace librets;

#define CLASS CurlSlist

CLASS::CLASS()
{
    mSlist = 0;
}

CLASS::~CLASS()
{
    free_all();
}

void CLASS::append(const char * aString)
{
    curl_slist * temp = curl_slist_append(mSlist, aString);
    if (temp == 0)
    {
        throw RetsException("Unable to append to curl_slist");
    }
    mSlist = temp;
}

void CLASS::free_all()
{
    if (mSlist != 0)
    {
        curl_slist_free_all(mSlist);
        mSlist = 0;
    }
}

const curl_slist * CLASS::slist()
{
    return mSlist;
}

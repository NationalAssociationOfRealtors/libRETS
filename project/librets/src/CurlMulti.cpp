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

#include <sstream>
#include "librets/CurlMulti.h"
#include "librets/CurlEasy.h"
#include "librets/RetsException.h"
#include "librets/str_stream.h"

using namespace librets;
using std::string;
using std::ostringstream;

#define CLASS CurlMulti

void CLASS::CurlAssert(CURLMcode errorCode, const string & prefix)
{
    if (errorCode != CURLM_OK)
    {
        const char * curlError = curl_multi_strerror(errorCode);
        ostringstream message;
        if (!prefix.empty())
        {
            message << "Could not " << prefix << ": ";
        }
        message << "curl error #" << errorCode  << " (" << curlError
            << ")";
        throw RetsException(message.str());
    }
}

CLASS::CLASS()
{
    mMultiHandle = curl_multi_init();
    if (!mMultiHandle)
    {
        throw RetsException("Could not allocate Curl multi handle");
    }
}

CLASS::~CLASS()
{
    curl_multi_cleanup(mMultiHandle);
}

void CLASS::AddEasy(CurlEasy * curlEasy)
{
    CurlAssert(curl_multi_add_handle(mMultiHandle, curlEasy->GetEasyHandle()));
}

void CLASS::RemoveEasy(CurlEasy * curlEasy)
{
    CurlAssert(curl_multi_remove_handle(mMultiHandle, curlEasy->GetEasyHandle()));
}

CURLM * CLASS::GetMultiHandle()
{
    return mMultiHandle;
}

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

#ifndef LIBRETS_CURL_MULTI_H
#define LIBRETS_CURL_MULTI_H

#include <string>
#include "librets/curl.h"

namespace librets {

class CurlEasy;
    
class CurlMulti
{
  public:
    CurlMulti();
    ~CurlMulti();
    
    void AddEasy(CurlEasy & curlEasy);

    void RemoveEasy(CurlEasy & curlEasy);
    
    CURLM * GetMultiHandle();
    
    void Perform();
    
    void Reset();
    
    bool StillRunning();
    
private:
    CurlMulti(const CurlEasy & curlEasy);
    
    void CurlAssert(CURLMcode errorCode, const std::string & prefix = "");

    CURLM * mMultiHandle;
    
    int mStillRunning;
};

}

#endif

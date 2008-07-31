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
/** 
 * @file CurlMulti.h
 * (Internal) Contains the Curl Http Multi interface class for use with libCURL.
 * This is a libRETS internal support class.
 */
/// @cond MAINTAINER

#include <string>
#include "librets/curl.h"

namespace librets {

class CurlEasy;

/**
 * (Internal) CurlMulti is a class that provides an interface to the libcurl "multi"
 * interface. It is used in support of the "streaming" enhancement to libRETS.
 */
class CurlMulti
{
  public:
    CurlMulti();
    ~CurlMulti();
    
    /**
     * Add a libcurl "easy" handle to this class.
     * @param curlEasy A reference to a CurlEasy object.
     */
    void AddEasy(CurlEasy & curlEasy);

    /**
     * Remove the provided libcurl "easy" handle from this class and libcurl.
     * @param curlEasy A reference to the CurlEasy object to remove.
     */
    void RemoveEasy(CurlEasy & curlEasy);
    
    /**
     * Returns the libcurl handle used by this object.
     * @return Pointer to CURLM (refer to the libcurl documentation).
     */
    CURLM * GetMultiHandle();
    
    /**
     * Perform a multi operation. The caller will need to handle the determination as
     * to whether enough data has arrived and if not, call Perform() again in a loop.
     * @throw RetsException
     */
    void Perform();
    
    /**
     * Reset for a new transaction.
     */
    void Reset();
    
    /**
     * Determine whether or not the current http request is still in progress with libcurl.
     * @return A boolean that if TRUE, indicates that the transaction is still in progress.
     */
    bool StillRunning();
    
private:
    CurlMulti(const CurlEasy & curlEasy);
    
    void CurlAssert(CURLMcode errorCode, const std::string & prefix = "");

    CURLM * mMultiHandle;
    
    int mStillRunning;
};

}
///@endcond
#endif

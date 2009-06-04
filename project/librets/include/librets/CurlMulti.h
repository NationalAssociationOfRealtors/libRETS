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
#include <vector>
#include "librets/curl.h"
#include "librets/CurlEasy.h"
#include <boost/shared_ptr.hpp>

namespace librets {


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
     * Add a libcurl "easy" handle to the multi handle and place it on the in use vector.
     * @param curlEasy A pointer to a CurlEasy object.
     */
    void AddEasy(CurlEasyPtr curlEasy);

    /**
     * Returns a pointer to a CurlEasy object that can be used for an http request.
     * @return Pointer to CurlEasy object.
     */
    CurlEasyPtr EasyFactory();
    
    /**
     * Add the "easy" handle back to the available queue.
     * @param curlEasy A pointer to a CurlEasy object.
     */
    void FreeEasy(CurlEasyPtr curlEasy);
    
    /**
     * Returns the curl cookie slist.
     * @return A pointer to a curl_slist structure.
     */
    curl_slist * GetCookieSlist();
    
    /**
     * Returns the libcurl handle used by this object.
     * @return Pointer to CURLM (refer to the libcurl documentation).
     */
    CURLM * GetMultiHandle();
    
    /**
     * Returns the number of requests that were running at the beginning of this iteration.
     * @return An integer containing the number of requests prior to this request.
     */
    int GetPriorRunning();

    /**
     * Returns the number of reqeusts still running.
     * @return An integer representing the number of requests in progress.
     */
    int GetStillRunning();
    
    /**
     * Perform a multi operation. The caller will need to handle the determination as
     * to whether enough data has arrived and if not, call Perform() again in a loop.
     * @throw RetsException
     */
    void Perform();

    /**
     * Remove the provided libcurl "easy" handle from this class and libcurl.
     * @param curlEasy A reference to the CurlEasy object to remove.
     */
    void RemoveEasy(CurlEasyPtr curlEasy);
    
    /**
     * Determine whether or not the current http request is still in progress with libcurl.
     * @return A boolean that if TRUE, indicates that the transaction is still in progress.
     */
    bool StillRunning();
 
private:
    CurlMulti(const CurlEasy & curlEasy);
    
    void CurlAssert(CURLMcode errorCode, const std::string & prefix = "");

    CURLM * mMultiHandle;
    
    CURLSH * mShareHandle;
    
    CurlEasyVector mCurlEasyInUse;
    CurlEasyVector mCurlEasyAvailable;
    
    int mPriorRunning;
    int mStillRunning;
    
    curl_slist * mCookieList;
};

}
///@endcond
#endif

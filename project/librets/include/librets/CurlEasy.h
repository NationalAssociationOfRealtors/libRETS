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

#ifndef LIBRETS_CURL_EASY_H
#define LIBRETS_CURL_EASY_H

#include <string>
#include "librets/curl.h"

namespace librets {

class CurlEasy
{
  public:
    CurlEasy();
    ~CurlEasy();
    
    void SetVerbose(bool verbose);
    
    void SetDebugData(void * debugData);
    
    void SetDebugFunction(curl_debug_callback debugFunction);
    
    void SetHttpAuth(long httpAuth);
    
    void SetCookieFile(std::string cookieFile);
    
    void SetWriteData(void * writeData);
    
    void SetWriteFunction(curl_write_callback writeFunction);
    
    void SetWriteHeaderData(void * headerData);
    
    void SetWriteHeaderFunction(curl_write_callback headerFunction);
    
    void SetUserCredentials(std::string username, std::string password);
    
    void SetHttpGet(bool httpGet);
    
    void SetPostFields(std::string postFields);
    
    void SetHttpHeaders(const curl_slist * httpHeaders);
    
    void SetUrl(std::string url);
    
    void Perform();
    
    long GetResponseCode();
    
    CURL * GetEasyHandle();
    
  private:
    CurlEasy(const CurlEasy & curlEasy);

    void CurlAssert(CURLcode errorCode, const std::string & prefix = "");
    
    void SetErrorBuffer(char * mCurlErrorBuffer);

    bool mUseErrorBuffer;
    CURL * mCurl;
    char mErrorBuffer[CURL_ERROR_SIZE];
    
    // Need to keep copies of strings we pass to curl, as it just stores
    // a pointer (i.e. it does not copy the contents into the library)
    std::string mCookieFile;
    std::string mUserpwd;
    std::string mPostFields;
    std::string mUrl;
};
    
}

#endif

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
#ifndef LIBRETS_CURL_HTTP_CLIENT_H
#define LIBRETS_CURL_HTTP_CLIENT_H

#include <sstream>
#include <istream>
#include "librets/curl.h"
#include "librets/std_forward.h"
#include "librets/RetsHttpClient.h"
#include "librets/CurlSlist.h"
#include "librets/RetsException.h"

namespace librets {

typedef boost::shared_ptr<std::stringstream> stringstreamPtr;

class CurlHttpClient : public RetsHttpClient
{
  public:
    CurlHttpClient();
    
    virtual ~CurlHttpClient();

    virtual void SetUserCredentials(std::string userName,
                                    std::string password);

    virtual void SetDefaultHeader(std::string name, std::string value);
    
    virtual void ClearDefaultHeader(std::string name);
    
    virtual void SetUserAgent(std::string userAgent);

    virtual RetsHttpResponsePtr DoRequest(RetsHttpRequestPtr request);

  private:
    void CurlAssert(const RetsExceptionContext & context, CURLcode errorCode,
                    bool useErrorBuffer = true);
    static size_t WriteData(void * buffer, size_t size, size_t nmemb,
                            void * userData);
    static size_t WriteHeader(void * buffer, size_t size, size_t nmemb,
                              void * userData);
    void GenerateHeaderSlist();

    CURL * mCurl;
    char * mCurlErrorBuffer[CURL_ERROR_SIZE];
    CurlSlist mHeaders;
    CurlHttpResponsePtr mResponse;

    // Need to keep copies of strings we pass to curl, as it just stores
    // a pointer (i.e. it does not copy the contents into the library)
    std::string mCurlUserpwd;
    std::string mCurlUrl;
    std::string mQueryString;
    StringMap mDefaultHeaders;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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
#include "librets/CurlEasy.h"
#include "librets/CurlMulti.h"
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
    
    ~CurlHttpClient();
    
    virtual void SetUserCredentials(std::string userName,
                                    std::string password);

    virtual void SetDefaultHeader(std::string name, std::string value);
    
    virtual std::string GetDefaultHeader(std::string name) const;

    virtual void ClearDefaultHeader(std::string name);
    
    virtual void SetUserAgent(std::string userAgent);
    
    virtual std::string GetUserAgent() const;

    virtual RetsHttpResponsePtr StartRequest(RetsHttpRequest * request);
    
    virtual bool ContinueRequest();
    
    virtual void SetLogger(RetsHttpLogger * logger);
    
    virtual int GetResponseCode();
    

  private:
    static size_t StaticWriteData(char * buffer, size_t size, size_t nmemb,
                                  void * userData);
        
    size_t WriteData(char * buffer, size_t size, size_t nmemb);

    static size_t StaticWriteHeader(char * buffer, size_t size, size_t nmemb,
                                    void * userData);
    
    size_t WriteHeader(char * buffer, size_t size, size_t nmemb);
    
    static int StaticDebug(CURL * handle, curl_infotype type, char * data,
                           size_t size, void * userData);
    
    int Debug(CURL * handle, curl_infotype type, char * data, size_t size);
    
    void GenerateHeadersSlist(const StringMap & requestHeaders);
    
    CurlEasy mCurl;

    CurlMulti mCurlMulti;
        
    StringMap mDefaultHeaders;                                                 

    CurlSlist mHeaders;
    
    CurlHttpResponsePtr mResponse;
    
    RetsHttpLogger * mLogger;
    
    int mResponseCode;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

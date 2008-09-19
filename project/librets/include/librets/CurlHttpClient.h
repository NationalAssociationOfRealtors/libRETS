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
/** 
 * @file CurlHttpClient.h
 * (Internal) Contains the Curl Http interface class for use with libCURL.
 */
/// @cond MAINTAINER

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

/**
 * (Internal) CurlHttpClient is a class that wraps client side interaction with
 * libcurl.
 */
class CurlHttpClient : public RetsHttpClient
{
  public:
    CurlHttpClient();
    
    ~CurlHttpClient();
    
    virtual void SetUserCredentials(std::string userName,
                                    std::string password);

    std::string GetCookie(const char * name);

    virtual void SetDefaultHeader(std::string name, std::string value);
    
    virtual std::string GetDefaultHeader(std::string name) const;

    virtual void ClearDefaultHeader(std::string name);
    
    virtual void SetUserAgent(std::string userAgent);
    
    virtual std::string GetUserAgent() const;
    
    /**
     * Start an HTTP request using the cURL multi-interface. This will perform
     * the first invocation if cURL using the multi-interface for the current
     * transaction.
     * @param request A pointer to a RetsHttpRequest to be sent to cURL.
     */
    virtual RetsHttpResponsePtr StartRequest(RetsHttpRequest * request);
    
    /**
     * Continue the current in process request. This is part of the streaming
     * interface and should only be called by CurlHttpClient::GetResponseCode and
     * CurlStream::read and CurlStream::eof. Data may be transferred with this request.
     * @return A boolean that if TRUE indicates that there is more data to process.
     * @see CurlHttpClient::GetResponseCode
     * @see CurlStream::eof
     * @see CurlStream::read
     */
    virtual bool ContinueRequest();
    
    virtual void SetLogger(RetsHttpLogger * logger);
    
    /**
     * Return the HTTP response code. With the streaming interface, this will
     * have the side-effect of completing the in process transaction.
     * @return An integer representing the HTTP response code.
     */
    virtual int GetResponseCode();
    
    virtual RetsHttpLogger* GetLogger() const;
    
    /**
     * Set the mode flags to be used.
     * @param An unsigned integer containing the mode flags.
     */
    virtual void SetModeFlags(unsigned int flags);
	
    /**
     * Set the proxy url and password.
     * @param url A string containing the URL of the proxy server.
     * @param password A string containing the password when the proxy
     * server requires authentication. Leave this as an empty string otherwise.
     */
	virtual void SetProxy(std::string url, std::string password);
    
    /**
     * Set the timeout for the Http transaction.
     * @param seconds An integer containing the number of seconds to set for the
     * timeout. A zero value will disable the timeout.
     */
    virtual void SetTimeout(int seconds);

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
    
    CurlSlist mCookies;
    
    bool mLogging;
    
    unsigned int mFlags;
};

};
///@endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

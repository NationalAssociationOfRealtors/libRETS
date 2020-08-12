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
    
    virtual RetsHttpLogger* GetLogger() const;
    
    /**
     * Set the mode flags to be used.
     * @param flags An unsigned integer containing the mode flags.
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

    /**
     * Get the path to the CACert.pem file
     * @return A string containing the path to cacert.pem
     */
    virtual std::string GetCACertPath() { return mCACertPath;};

    /**
     * Set the path to the CACert.pem file
     * @param A string containing the path to cacert.pem
     */
    virtual void SetCACertPath(std::string cacert_path) {mCACertPath = cacert_path;};

  private:
  
    static size_t StaticWriteData(char * buffer, size_t size, size_t nmemb,
                                  void * userData);
        
    static size_t StaticWriteHeader(char * buffer, size_t size, size_t nmemb,
                                    void * userData);
    
    static int StaticDebug(CURL * handle, curl_infotype type, char * data,
                           size_t size, void * userData);
    
    void GenerateHeadersSlist(const StringMap & requestHeaders);

    CurlMulti mCurlMulti;
        
    StringMap mDefaultHeaders;                                                 

    CurlSlist mHeaders;

    RetsHttpLogger * mLogger;
    
    CurlSlist mCookies;
    
    bool mLogging;
    
    std::string mCACertPath;

    std::string mUrl;
    
    std::string mUserName;
    
    std::string mPassword;
    
    std::string mProxyUrl;
    
    std::string mProxyPassword;
    
    int mTimeout;
    
    unsigned int mFlags;
};

/**
 * (Internal) CurlHttpClientPrivate is a class that wraps the three libRETS classes
 * that make up a single request and response. This becomes libCURL's private data for
 * the particular easy handle that invokes this request.
 */
class CurlHttpClientPrivate
{
  public:
    /**
     * Construct the CurlHttpClientPrivate class.
     * @param request A pointer to the request object.
     * @param response A CurlHttpResponsePtr referencing to where the response will be returned.
     * @param client A pointer to the CurlHttpClient class that controls the transaction.
     */
    CurlHttpClientPrivate(RetsHttpRequest * request,
                          CurlHttpResponsePtr response,
                          CurlHttpClient * client, CurlEasyPtr curlEasy)
                    : mRequest(request)
                    , mResponse(response)
                    , mClient(client)
                    , mCurlEasy(curlEasy)
    {
    };
    
    ~CurlHttpClientPrivate() {};
    /**
     * Returns a pointer to the RetsHttpRequest object for this request.
     * @return A pointer to the RetsHttpRequest.
     */
    RetsHttpRequest * GetRequest()
    {
        return mRequest;
    };
    
    /**
     * Return the reference to the response object associated with this request.
     * @return A CurlHttpResponsePtr.
     */
    CurlHttpResponsePtr GetResponse()
    {
        return mResponse;
    };
    
    /**
     * Return a pointer to the CurlHttpClient that controls this request.
     * @return A pointer to the CurlHttpClient.
     */
    CurlHttpClient * GetClient()
    {
        return mClient;
    };

    /**
     * Return a pointer to the CurlEasy object associated with this request.
     * @return A pointer to CurlEasy
     */
    CurlEasyPtr GetCurlEasy()
    {
        return mCurlEasy;
    };
    
  private:
    RetsHttpRequest * mRequest;
    CurlHttpResponsePtr mResponse;
    CurlHttpClient * mClient;
    CurlEasyPtr mCurlEasy;
};

};

///@endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

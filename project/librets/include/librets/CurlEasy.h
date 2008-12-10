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
/** 
 * @file CurlEasy.h
 * (Internal) Contains the Curl Http Easy interface class for use with libCURL.
 */
/// @cond MAINTAINER

#include <string>
#include "librets/curl.h"
#include "librets/http_forward.h"

namespace librets {

/**
 * (Internal) CurlEasy is a class that provides an interface to the libcurl "easy"
 * interface. 
 */
class CurlEasy
{
  public:
    CurlEasy();
    ~CurlEasy();
    
    /**
     * Enable verbose logging.
     * @param verbose TRUE to enable logging, FALSE to disable it.
     */
    void SetVerbose(bool verbose);
    
    /**
     * Provide to cURL a pointer to the opaque data that cURL in turn
     * will use for the debugging callbacks.
     * @param debugData A pointer to the opaque data to be given to cURL.
     * Typically this is the "this" pointer for the class implementing the 
     * callback.
     */
    void SetDebugData(void * debugData);
    
    /**
     * Provide to cURL a reference to the method that cURL should
     * invoke when performing the logging/debug function.
     * @param debugFunction A static reference to the callback function.
     */
    void SetDebugFunction(curl_debug_callback debugFunction);
    
    /**
     * Indicate to cURL the authentication methodology. See the libCURL 
     * documentation.
     * @param httpAuth The cURL authentication code.
     */
    void SetHttpAuth(long httpAuth);
    
    /**
     * Indicate to cURL the name of the file to which cookies will be stored.
     * @param cookieFile A string containing the name of the cookie file.
     */
    void SetCookieFile(std::string cookieFile);
        
    /**
     * Provide to cURL a pointer to the opaque data that cURL in turn
     * will use for the WriteData callbacks.
     * @param writeData A pointer to the opaque data to be given to cURL.
     * Typically this is the "this" pointer for the class implementing the 
     * callback.
     */
    void SetWriteData(void * writeData);

    /**
     * Provide to cURL a reference to the method that cURL should
     * invoke when trying to write data it accepts from the network.
     * @param writeFunction A static reference to the callback function.
     */
    void SetWriteFunction(curl_write_callback writeFunction);
    
    /**
     * Provide to cURL a pointer to the opaque data that cURL in turn
     * will use for the WriteHeaderData callbacks.
     * @param headerData A pointer to the opaque data to be given to cURL.
     * Typically this is the "this" pointer for the class implementing the 
     * callback.
     */
    void SetWriteHeaderData(void * headerData);

    /**
     * Provide to cURL a reference to the method that cURL should
     * invoke when trying to write data it accepts from the network.
     * @param headerFunction A static reference to the callback function.
     */
    void SetWriteHeaderFunction(curl_write_callback headerFunction);
    
    /**
     * Provide to cURL the user name and the password to be used with the
     * selected authentication method.
     * @param username A string containing the user name.
     * @param password A string containing the password.
     */
    void SetUserCredentials(std::string username, std::string password);
    
    /**
     * Indicate to cURL whether or not the GET or POST method should be used.
     * @param httpGet A boolean when TRUE sets the GET method; when FALSE set the
     * POST method.
     */ 
    void SetHttpGet(bool httpGet);
    
    /**
     * Provide to cURL a string containing the fields to be used with the POST.
     * @param postFields A string containg the fields to be used with the POST.
     */
    void SetPostFields(std::string postFields);
    
    /**
     * Provide to cURL a list of headers and values to be sent as part of the 
     * http transaction.
     * @param httpHeaders A list of headers. See the libCURL documentation.
     */
    void SetHttpHeaders(const curl_slist * httpHeaders);
    
    /**
     * Provide to cURL a string containing the URL to which the http transaction
     * will be sent.
     * @param url A string containing a URL.
     */
    void SetUrl(std::string url);
    
    /**
     * Provide to cURL a string containing the URL of the proxy server.
     * The URL should include the proxy port.
     * @param url A string containing the proxy URL.
     */
    void SetProxyUrl(std::string url);
    
    /**
     * Provide to cURL the proxy authentication password if required.
     * @param password A string containing the proxy password.
     */
    void SetProxyPassword(std::string password);
    
    /**
     * Provide to cURL a pointer to the private data associated with this request.
     * @param data A pointer to the private data.
     */
    void SetPrivateData(void * data);
    
    /**
     * Provide to cURL a reference to the shared object.
     * @param shared A pointer to a CURLSH handle.
     */
    void SetShareHandle(CURLSH * shared);
    
    /**
     * Provide to cURL an integer containing a timeout in seconds.
     * @param seconds An integer containing the number of seconds to establish
     * as a timeout.
     */
    void SetTimeout(int seconds);
    
    /**
     * Tell cURL whether or not to verify SSL connections.
     * @param verify A boolean that if TRUE (default), indicates that the connection
     * should be verified.
     */
    void SetSSLVerify(bool verify);
    
    /**
     * Tell cURL to perform the current request.
     */
    void Perform();
    
    /**
     * Obtain the response code from cURL for the last transaction. Because of the
     * use of the cURL multi-interface, it is possible for the status to be zero until
     * the entire transaction has been satisfied. It is assumed, therefore, that when
     * this method is called, the caller desires the transaction to be completed. This
     * method will therefore continue the request until a non-zero response is returned
     * by cURL.
     * @returns An integer representation of the http response code.
     */
    long GetResponseCode();
    
    /**
     * Get the current state of the verbose/logging flag.
     * @returns a boolean when TRUE, indicates that logging/verbosity is enabled.
     */
    bool GetVerbose();
    
    /**
     * Get the current handle to the cURL "Easy" structure. See the libCURL documentation.
     */
    CURL * GetEasyHandle();
    
    /**
     * Get a pointer to the cookie list from cURL.
     * @return A pointer to the curl_slist. See the libCURL documentation.
     */
    curl_slist * GetCookieSlist();

    /**
     * Get the private data associated with this request.
     * @return A pointer to the private data.
     */
    void * GetPrivateData();

    /**
     * Get the current curl library version information.
     * @return A string representing the libCURL verison.
     */
    std::string GetVersion();
    
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
    std::string mProxyUrl;
    std::string mProxyPassword;
    bool mVerbose;
    
    std::string mCurlVersion;
};
    
}
/// @endcond
#endif

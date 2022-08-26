/*
 * Copyright (C) 2005-2011 National Association of REALTORS(R)
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

#ifndef LIBRETS_RETS_SESSION_H
#define LIBRETS_RETS_SESSION_H
/** 
 * @file RetsSession.h
 * Contains the RetsSession class definition.
 */
#include <string>
#include <fstream>
#include "librets/protocol_forward.h"
#include "librets/RetsHttpRequest.h"
#include "librets/http_forward.h"
#include "librets/RetsVersion.h"
#include "librets/EncodingType.h"
#include "librets/ObjectDescriptor.h"
#include "librets/UserAgentAuthType.h"
#include "librets/UserAgentAuthCalculator.h"
#include "librets/error_forward.h"
#include "MetadataLoader.h"

#ifdef LIBRETS_THREAD_SAFE
#include <boost/thread/thread.hpp>
#include "librets/winundef.h"
#endif

/**
 * The main librets namespace.  See RetsSession.
 */
namespace librets {

/**
 * RetsSession contains the API that is the main controlling
 * interface to the RETS server.
 */
class RetsSession : public MetadataLoader
{
  public:
    /**
     * Create a new RETS session with a login URL.
     *
     * @param loginUrl The login URL to a RETS server
     * @throws RetsException if an error occurs.
     */
    RetsSession(std::string loginUrl);

    ~RetsSession();
    /**
     * Returns the LoginUrl that was used when this session was constructed.
     *
     * @return the login url
     */
    std::string GetLoginUrl() const;

    /**
     * Logs into the RETS server.
     *
     * @param userName user name
     * @param password password
     * @return True, if login is successful.
     * @throws RetsException if an error occurs.
     */
    bool Login(std::string userName, std::string password);

    /**
     * Logs into the RETS server with optional brokercode and saved
     * metadata timestamp.
     *
     * @param userName user name
     * @param password password
     * @param brokerCode Broker Code
     * @param savedMetadataTimestamp The saved metadata timestamp (RETS 1.7+)
     *
     * @return True, if login is successful.
     * @throws RetsException if an error occurs.
     */
    bool Login(std::string userName, 
                std::string password, 
                std::string brokerCode,
                std::string savedMetadataTimestamp);

    /**
     * Returns the login response from the last successful login transaction.
     * If the last login trasaction was not successful, NULL is returned.
     *
     * @return Login response
     * @throws RetsEXception if an error occurs.
     */
    LoginResponse * GetLoginResponse() const;
    
    /**
     * Returns the capability URLs from the last successful login transaction.
     * If the last login transaction was not successful, NULL is returned.
     *
     * @return Capability URLs
     * @throws RetsException if an error occurs.
     */
    CapabilityUrls * GetCapabilityUrls() const;
    
    /**
     * Returns the contents of the action URL from the last successful
     * login.
     *
     * @return the contents of the action URL
     * @throws RetsException if an error occurs.
     */
    std::string GetAction();
    
    /**
     * Create a new search request with correct query type based on the 
     * detected version.
     *
     * @param searchType RETS resource name
     * @param searchClass RETS class name
     * @param query DMQL(2) query
     */
    SearchRequestAPtr CreateSearchRequest(std::string searchType, 
                                          std::string searchClass,
                                          std::string query);
    
    /**
     * Performs a search on the server.
     *
     * @param request search request parameters
     * @return Search result set
     * @throws RetsException if an error occurs.
     */
    SearchResultSetAPtr Search(SearchRequest * request);

    /**
     * Performs a search on the server, content is copied to output stream
     * as the raw unparsed RETS data.
     *
     * @param request search request parameters
     * @param outputStream  the destination stream
     * @return nothing
     * @throws RetsException if an error occurs.
     */
    void Search(SearchRequest * request, std::ostream & outputStream);
    
    /// @cond MAINTAINER
    /**
     * Performs a search on the server and return the data as BinaryDataPtr.
     * This API is primarily used by the SWIG bound languages.
     *
     * @param request search request parameters
     * @return BinaryDataAPtr containing the search results.
     * @throws RetsException if an error occurs.
     */
    BinaryDataAPtr Search_(SearchRequest * request);
    
    /**
     * Performs a search on the server and return the data as a stream.
     * This API is primarily used by the SWIG bound languages.
     *
     * @param request search request parameters
     * @return BinaryDataAPtr containing the search results.
     * @throws RetsException if an error occurs.
     */
    istreamPtr SearchStream(SearchRequest * request);
    /// @endcond

#ifdef DOXYGEN
    /**
     * Performs a search on the server and returns the raw RETS data into 
     * a java byte array. This API is for Java only.
     *
     * @param request search request parameters
     * @return a Java byte array containing the search results as raw RETS data.
     */
    public byte [] SearchAsArray(SearchRequest request);

    /**
     * Performs a search on the server and returns the raw RETS data into 
     * a PHP string. This API is for PHP only.
     *
     * @param request search request parameters
     * @return a PHP string containing the search results as raw RETS data.
     */
    string SearchAsString(SearchRequest request);
#endif

    /** 
     * Provides access to the last HTTP response from DoRequest.
     * @return A reference to the HTTP response
     */
    const RetsHttpResponsePtr& GetHttpResponse() const;

    /**
     * Causes the session to hold a reference to the last HTTPResponse objects
     * after DoRequest(). This can be useful for debugging error conditions in
     * the HTTP layer. By default HTTP responses are not cached.
     */
    void EnableResponseCaching();

    /**
     * @deprecated
     * Create a new server information request (RETS 1.7).
     *
     * @param  resourceName RETS resource name
     * @param className RETS class name
     * @param standardNames <code>true</code> indicates Standard Names,
     * <code>false</code> indicates System Names (default).
     * @return Server Information Response.
     */
    ServerInformationResponseAPtr GetServerInformation(std::string resourceName = "", 
                                                std::string className = "",
                                                bool standardNames = false);

    /**
     * Returns the metadata for this server.  Only valid after logging
     * in.  This object is owned by the RetsSession and should not need
     * to be deleted by the user.
     *
     * @return The metadata for this server
     * @throws RetsException if an error occurs.
     */
    RetsMetadata * GetMetadata();
     
    /**
     * Copies the metadata for this server to the output stream as 
     * raw unparsed RETS data.
     * Only valid after logging in.  
     * @param outputStream  the destination stream
     * @return nothing
     * @throws RetsException if an error occurs.
     */
    void GetMetadata(std::ostream & outputStream);

    /// @cond MAINTAINER
    /**
     * Copies the metadata on the server and returns the data as BinaryDataPtr.
     * This API is primarily used by the SWIG bound languages.
     *
     * @return BinaryDataAPtr containing the metadata.
     * @throws RetsException if an error occurs.
     */
    BinaryDataAPtr GetMetadata_();
    /// @endcond
   
#ifdef DOXYGEN
    /**
     * Gets the metadata on the server and returns the raw RETS data into 
     * a java byte array. This API is for Java only.
     *
     * @return a Java byte array containing the search results as raw RETS data.
     */
    public byte [] GetMetadataAsArray();

    /**
     * Gets the metadata on the server and returns the raw RETS data into 
     * a PHP string. This API is for PHP only.
     *
     * @return a PHP string containing the search results as raw RETS data.
     */
    string GetMetadataAsString();
#endif

    /**
     * Returns true if metadata is retrieved incrementally.
     *
     * @return true if metadata is retreived incrementally.
     */
    bool IsIncrementalMetadata() const;
    
    /**
     * Changes how metadata is fetched.  If true, metadata is retrieved
     * incrementally, as needed.  If false, metadata is retreived all at once
     * right after logging in.  The default is true (incremental).
     */
    void SetIncrementalMetadata(bool incrementalMetadata);
    
    /**
     * Fetches media from the server.
     * @param request A pointer to the GetObjectRequest containing the
     * objects to fetch with this request.
     */
    GetObjectResponseAPtr GetObject(GetObjectRequest * request);

    /**
     * Logs out of the server.
     *
     * @return Logout information
     * @throws RetsException if an error occurs.
     */
    LogoutResponseAPtr Logout();
    
    /**
     * Sets the HTTP user agent.
     *
     * @param userAgent HTTP user agent string.
     */
    void SetUserAgent(std::string userAgent);
    
    /** Default user agent, for librets. */
    static const char * DEFAULT_USER_AGENT;
    
    /**    
     * Use the HTTP GET method for GetMetadata, Seach, and GetObject
     * requests.  The default is to use POST.
     *
     * @param useHttpGet <code>true</code> for HTTP GET instead of POST
     */
    void UseHttpGet(bool useHttpGet);

    /**
     * Accepts a pointer to an instance of RetsHttpLogger to be used
     * for logging.
     *
     * @param logger A pointer to the RetsHttpLogger to use.
     */
    void SetHttpLogger(RetsHttpLogger * logger);

    /**
     * Accepts a string that contains the name of a logfile. Http
     * logging will be enabled and the output will go to the named
     * file.
     *
     * @param logfile  A string containing the name of the log file.
     */
    void SetHttpLogName(std::string logfile);
    
    /**
     * Returns a pointer to the instances of the RetsHttpLogger that
     * will be/is being used.
     */
    RetsHttpLogger* GetHttpLogger() const;
    
    /**
     * Return the RETS version to use when first logging in.
     *
     * @return RETS version to use when first logging in
     */
    RetsVersion GetRetsVersion() const;
    
    /**
     * Sets the RETS version to use when first logging in.
     *
     * @param retsVersion RETS version to use when first logging in.
     */
    void SetRetsVersion(RetsVersion retsVersion);
    
    /** The default RETS version when logging in, 1.5 */
    static const RetsVersion DEFAULT_RETS_VERSION;
    
    /**
     * Returns the RETS version detected by the login transaction and
     * used by all subsequent transactions.
     *
     * @return the detected RETS version
     */
    RetsVersion GetDetectedRetsVersion() const;
    /**
     * Returns the RETS version as a printable string.
     *
     * @param retsVersion RETS version to convert to a string.
     *
     * @return String representation of RETS version.
     */
    std::string RetsVersionToString(RetsVersion retsVersion);

    /**
     * Changes the error handler. The default error handler throws exceptions
     * in error conditions, which may not be desireable for some clients.
     *
     * @param errorHandler the new error handler.
     */
    void SetErrorHandler(RetsErrorHandler * errorHandler);
    
    /// @cond MAINTAINER
    /**
     * Sets the method that will be used to collect the metadata.
     * @param collector A pointer to the MetadtaElementCollector.
     */
    virtual void SetCollector(MetadataElementCollectorPtr collector);
    /// @endcond
    
    /// @cond MAINTAINER
    /**
     * Fetch the specified metadata.
     * @param type The MetadataElement::Type for this request.
     * @param level A string containing the level for the current type.
     */
    virtual void LoadMetadata(MetadataElement::Type type,
                              std::string level);
    /// @endcond
    
    /**
     * Sets the User-Agent authorzation type.  
     *
     * @param type Either USER_AGENT_AUTH_RETS_1_7 or USER_AGENT_AUTH_INTEREALTY
     */
    void SetUserAgentAuthType(UserAgentAuthType type);

    /**
     * Returns the current User-Agent authorization type.
     */
    UserAgentAuthType GetUserAgentAuthType() const;
    
    /**
     * Sets the User-Agent password.
     */
    void SetUserAgentPassword(std::string userAgentPassword);
    
    /**
     * Sets the data encoding for the data to be returned by a Search. Thiss
     * is session wide,
     *
     * @param encoding RETS_XML_DEFAULT_ENCODING,
     *                 RETS_XML_ISO_ENCODING or RETS_XML_UTF8_ENCODING.
     */
    void SetDefaultEncoding(EncodingType encoding);

    /**
     * Sets the value of the flag that indicates everything should be logged,
     * including everything from GetObject().
     *
     * @param logging TRUE indicates to log everything, FALSE
     * indicates to not log GetObject() requests.
     */
    void SetLogEverything(bool logging);
    
    /**
     * Set the mode flags for the current session.
     *
     * @param flags A logical OR of the flags to be set.
     */
    void SetModeFlags(unsigned int flags);
    
    /**
     * Set the html proxy information.
     *
     * @param url A string containing the url of the proxy server. If non-standard ports
     * are used, it should be part of the url as in: http://demo.crt.realtors.org:6103.
     * @param password A string containing the proxy password if one is needed. This can be of
     * the form "userid:password".
     */
     void SetProxy(std::string url, std::string password);
     
     /**
      * Set a timeout value for the http transaction.
      *
      * @param seconds An integer containing the number of seconds to set for the timeout. If
      * zero, then no timeout is established and the default libCURL timeout will be in effect.
      */
    void SetTimeout(int seconds);
    
    /// @cond MAINTAINER
    /**
     * Cleans up/frees some underlying resources including the HTTP
     * interface.  This method is only really intended to be called by
     * languages that have swig bindings so their final garbage
     * collections work properly.  C++ users should not need this
     * method.
     *
     * The RetsSession object should be considered unusuable after this
     * call.
     */
    void Cleanup();
    /// @endcond
    
    /**
     * Return the version of libRETS.
     * @return A string representing the libRETS version information.
     */
    static std::string GetLibraryVersion();
    
    /**
     * Create a new update equest.
     *
     * @param resourceName RETS resource name
     * @param className RETS class name
     */
    UpdateRequestAPtr CreateUpdateRequest(std::string resourceName, 
                                          std::string className);
    
    /**
     * Performs an update on the server.
     *
     * @param request search request parameters
     * @return Search result set
     * @throws RetsException if an error occurs.
     */
    UpdateResponseAPtr Update(UpdateRequest * request);
    
    /**
     * RETS 1.8
     * Perform the GetPayloadList transaction
     *
     * @param metadataID The MetadataID for the payload list
     * @return PayloadList result set
     * @throws RetsException if an error occurs.
     */
    PayloadListResultSetAPtr GetPayloadList(std::string metadataID);

    /**
     * Mode flags for the current session.
     * MODE_CACHE is used in streaming mode to enable the row cache.
     * MODE_NO_STREAM will disable the streaming mode.
     * MODE_NO_EXPECT will disable the Expect: 100-Continue http header.
     * MODE_NO_SSL_VERIFY will disable verification of the certificate during SSL connections.
     */
    static const int    MODE_CACHE;
    static const int    MODE_NO_STREAM;
    static const int    MODE_NO_EXPECT;
    static const int    MODE_NO_SSL_VERIFY;
    
private:
    static const char * HTTP_EXPECT_HEADER;
    static const char * RETS_REQUEST_ID_HEADER;
    static const char * RETS_SESSION_ID_HEADER;
    static const char * RETS_VERSION_HEADER;
    static const char * RETS_UA_AUTH_HEADER;
    static const char * RETS_1_0_STRING;
    static const char * RETS_1_5_STRING;
    static const char * RETS_1_7_STRING;
    static const char * RETS_1_7_2_STRING;
    static const char * RETS_1_8_STRING;
    static const char * RETS_1_8_0_STRING;
    static const char * RETS_1_9_STRING;
    static const char * RETS_1_9_0_STRING;
    
    void RetrieveAction();

    void InitializeMetadata();
    
    void RetrieveFullMetadata();

    void AssertSuccessfulResponse(RetsHttpResponsePtr response,
                                  std::string url);
    
    RetsVersion RetsVersionFromString(std::string versionString);
    
    std::string MetadataTypeToString(MetadataElement::Type type);

    RetsHttpResponsePtr DoRequest(RetsHttpRequest * request);
    
    RetsHttpClientPtr mHttpClient;

    std::string mLoginUrl;

    LoginResponsePtr mLoginResponse;
    
    CapabilityUrlsPtr mCapabilityUrls;

    std::string mAction;

    RetsMetadataPtr mMetadata;
    
    MetadataElementCollectorPtr mLoaderCollector;
    
    bool mIncrementalMetadata;
    
    RetsHttpRequest::Method mHttpMethod;
    
    RetsVersion mRetsVersion;
    
    RetsVersion mDetectedRetsVersion;

    RetsErrorHandler * mErrorHandler;
    
    UserAgentAuthCalculator mUserAgentAuthCalculator;
    
    librets::EncodingType mEncoding;
    
    bool mLoggedIn;
    
    bool mLogEverything;

    ofstreamPtr mLogStream;

    RetsHttpLoggerPtr mLogger;
    
    int mTimeout;
    
    unsigned int mFlags;

    RetsHttpResponsePtr mHttpResponse;

    bool mHttpResponseCaching;

#ifdef LIBRETS_THREAD_SAFE
    boost::mutex mMutex;
#endif
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

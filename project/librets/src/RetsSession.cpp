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

#include <fstream>
#include <sstream>
#include <boost/algorithm/string.hpp>
#include "librets.h"
#include "librets/CurlHttpClient.h"
#include "librets/XmlMetadataParser.h"
#include "librets/DefaultMetadataCollector.h"
#include "librets/IncrementalMetadataFinder.h"
#include "librets/SearchRequest.h"
#include "librets/ServerInfoRequest.h"
#include "librets/ServerInfoResponse.h"
#include "librets/GetObjectRequest.h"
#include "librets/GetObjectResponse.h"
#include "librets/ObjectDescriptor.h"
#include "librets/PayloadListResultSet.h"
#include "librets/str_stream.h"
#include "librets/ExceptionErrorHandler.h"
#include "librets/RetsHttpException.h"
#include "boost/lexical_cast.hpp"

using namespace librets;
using namespace librets::util;
using std::ostringstream;
using std::string;
namespace ba = boost::algorithm;

typedef RetsSession CLASS;

const char * CLASS::DEFAULT_USER_AGENT  = "librets/" LIBRETS_VERSION;
const RetsVersion CLASS::DEFAULT_RETS_VERSION = RETS_1_5;
const char * CLASS::HTTP_EXPECT_HEADER  = "Expect";
const char * CLASS::RETS_REQUEST_ID_HEADER = "RETS-Request-ID";
const char * CLASS::RETS_SESSION_ID_HEADER = "RETS-Session-ID";
const char * CLASS::RETS_VERSION_HEADER = "RETS-Version";
const char * CLASS::RETS_UA_AUTH_HEADER = "RETS-UA-Authorization";
const char * CLASS::RETS_1_0_STRING     = "RETS/1.0";
const char * CLASS::RETS_1_5_STRING     = "RETS/1.5";
const char * CLASS::RETS_1_7_STRING     = "RETS/1.7";
const char * CLASS::RETS_1_7_2_STRING   = "RETS/1.7.2";
const char * CLASS::RETS_1_8_STRING     = "RETS/1.8";
const char * CLASS::RETS_1_8_0_STRING   = "RETS/1.8.0";

const int CLASS::MODE_CACHE             = 0x01;
const int CLASS::MODE_NO_STREAM         = 0x02;
const int CLASS::MODE_NO_EXPECT         = 0x04;
const int CLASS::MODE_NO_SSL_VERIFY     = 0x08;

static librets::EncodingType
GetEncoding(RetsHttpResponsePtr httpResponse, librets::EncodingType defaultEncoding)
{
    string content_type = httpResponse->GetContentType();
    if (content_type.empty())
        return defaultEncoding;

    // parse content type
    StringVector parameters;
    ba::split(parameters, content_type, ba::is_any_of(";"));
    StringVector::iterator i;
    for (i = parameters.begin(); i != parameters.end(); i++)
    {
        string parameter = ba::trim_copy(*i);
        string name;
        string value;
        if (splitField(parameter, "=", name, value) && (name == "charset"))
        {
            ba::trim(value);

            // Strip off leading and trailing quote, if it exists
            string::size_type quotePosition = value.find_first_of("\"");
            if (quotePosition == string::size_type(0))
            {
                value.erase(0, 1);
            }

            quotePosition = value.find_last_of("\"");
            if (quotePosition == (value.size() - 1))
            {
                value.erase(value.size() - 1, 1);
            }

            if (value == "utf-8")
                return RETS_XML_UTF8_ENCODING;
            else if (value == "iso-8859-1")
                return RETS_XML_ISO_ENCODING;
            else
                return defaultEncoding;
        }
    }

    return defaultEncoding;
}

CLASS::RetsSession(string login_url)
{
    mLoginUrl = login_url;
    mLoginResponse.reset();
    mCapabilityUrls.reset();
    mHttpMethod = RetsHttpRequest::POST;
    mHttpClient.reset(new CurlHttpClient());
    mHttpClient->SetUserAgent(DEFAULT_USER_AGENT);
    mRetsVersion = DEFAULT_RETS_VERSION;
    mErrorHandler = ExceptionErrorHandler::GetInstance();
    mIncrementalMetadata = true;
    mUserAgentAuthCalculator.SetUserAgentPassword("");
    mUserAgentAuthCalculator.SetUserAgentAuthType(USER_AGENT_AUTH_RETS_1_7);
    SetDefaultEncoding(RETS_XML_ISO_ENCODING);
    mFlags = 0;
    mLoggedIn = false;
    mTimeout = 0;
    /*
     * By default, do not log GetObject() transactions.
     */
    mLogEverything = false;
    mLogStream.reset(new std::ofstream());
    mHttpResponseCaching = false;
    mHttpResponse.reset();
}

CLASS::~RetsSession()
{
    if (mLogStream->is_open())
    {
        mLogStream->close();
    }
}

string CLASS::GetLoginUrl() const
{
    return mLoginUrl;
}

RetsHttpResponsePtr CLASS::DoRequest(RetsHttpRequest * request)
{
#ifdef LIBRETS_THREAD_SAFE
    boost::mutex::scoped_lock   lock(mMutex);
#endif
    
    if (mUserAgentAuthCalculator.HasAuthorizationValue())
    {
        mUserAgentAuthCalculator.SetRequestId("");
        mUserAgentAuthCalculator.SetSessionId("");
        if (mUserAgentAuthCalculator.GetUserAgentAuthType() == USER_AGENT_AUTH_RETS_1_7)
        {
            string requestId = mHttpClient->GetCookie(RETS_REQUEST_ID_HEADER);
            string sessionId = mHttpClient->GetCookie(RETS_SESSION_ID_HEADER);

            mUserAgentAuthCalculator.SetRequestId(requestId);
            mUserAgentAuthCalculator.SetSessionId(sessionId);
        }
        mUserAgentAuthCalculator.SetVersionInfo(
            mHttpClient->GetDefaultHeader(RETS_VERSION_HEADER));
        string headerValue = "Digest " + mUserAgentAuthCalculator.AuthorizationValue();
        request->SetHeader(RETS_UA_AUTH_HEADER, headerValue);
    }
    
    if (mFlags & MODE_NO_EXPECT && request->GetMethod() == RetsHttpRequest::POST)
    {
        mHttpClient->SetDefaultHeader(HTTP_EXPECT_HEADER, "");
    }

    RetsHttpResponsePtr httpResponse = mHttpClient->StartRequest(request);

    if (mHttpResponseCaching)
    {
        mHttpResponse = httpResponse;
    }
    
    /*
     * If we are not streaming, complete the request.
     */
    if (mFlags & MODE_NO_STREAM)
    {
        while (mHttpClient->ContinueRequest());
    }
    
    return httpResponse;
}

void CLASS::AssertSuccessfulResponse(RetsHttpResponsePtr response,
                                     string url)
{
    int responseCode = response->GetResponseCode();
    if (responseCode != 200) 
    {
        ostringstream message;
        message << "Could not get URL [" << url << "] - HTTP response code: "
                << responseCode
                << " " << response->GetAdditionalErrorText();
        throw RetsHttpException(responseCode, message.str());
    }
}

bool CLASS::Login(string user_name, string passwd)
{
    return Login(user_name, passwd, "", "");
}

bool CLASS::Login(string user_name, 
                    string passwd, 
                    string brokerCode, 
                    string savedMetadataTimestamp)
{
    mLoginResponse.reset();
    mCapabilityUrls.reset();
    mLoggedIn = false;
    mHttpClient->SetDefaultHeader(RETS_VERSION_HEADER,
                                  RetsVersionToString(mRetsVersion));
    RetsHttpRequest request;
    request.SetUrl(mLoginUrl);
    if (!brokerCode.empty())
    {
        request.SetQueryParameter("BrokerCode", brokerCode);
    }
    
    if (!savedMetadataTimestamp.empty())
    {
        request.SetQueryParameter("SavedMetadataTimestamp", savedMetadataTimestamp);
    }
    
    mHttpClient->SetUserCredentials(user_name, passwd);
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    
    if (httpResponse->GetResponseCode() == 401)
    {
        /* Just make sure we read the rest of the original response and free
           up the CurlEasy object before we issue the next request. It will
           make it possible to re-use the existing connection. */
        httpResponse->FinishResponse();
        httpResponse = DoRequest(&request);
        if (httpResponse->GetResponseCode() == 401)
        {
            return false;
        }
    }

    AssertSuccessfulResponse(httpResponse, mLoginUrl);
    mDetectedRetsVersion =
        RetsVersionFromString(httpResponse->GetHeader(RETS_VERSION_HEADER));
    mHttpClient->SetDefaultHeader(RETS_VERSION_HEADER,
                                  RetsVersionToString(mDetectedRetsVersion));
    
    mLoginResponse.reset(new LoginResponse);
    mLoginResponse->Parse(httpResponse->GetInputStream(), mDetectedRetsVersion,
        GetEncoding(httpResponse, mEncoding));
    if (mLoginResponse->GetRetsReplyCode() != 0)
        return false;
    
    mCapabilityUrls.reset(
        mLoginResponse->CreateCapabilityUrls(mLoginUrl).release());

    RetrieveAction();
    
    mLoggedIn = true;

    try
    {
        std::string seconds = mLoginResponse->GetTimeout();
        mTimeout = boost::lexical_cast<int>(seconds);
    }
    catch (boost::bad_lexical_cast &)
    {
        mTimeout = 0;
    }

    return true;
}

LoginResponse * CLASS::GetLoginResponse() const
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    return mLoginResponse.get();
}

CapabilityUrls * CLASS::GetCapabilityUrls() const
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    return mCapabilityUrls.get();
}

void CLASS::RetrieveAction()
{
    string actionUrl = mCapabilityUrls->GetActionUrl();
    if (actionUrl == "")
    {
        mAction = "";
        return;
    }

    RetsHttpRequest request;
    request.SetUrl(actionUrl);
    RetsHttpResponsePtr httpResponse(DoRequest(&request));

    AssertSuccessfulResponse(httpResponse, actionUrl);
    mAction = readIntoString(httpResponse->GetInputStream());
}

string CLASS::GetAction()
{
    return mAction;
}

RetsMetadata * CLASS::GetMetadata()
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    if (!mMetadata)
    {
        InitializeMetadata();
    }
    return mMetadata.get();
}

void CLASS::GetMetadata(std::ostream & outputStream)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    string getMetadataUrl = mCapabilityUrls->GetGetMetadataUrl();
    RetsHttpRequest request;
    request.SetUrl(getMetadataUrl);
    request.SetMethod(mHttpMethod);
    request.SetQueryParameter("Type", "METADATA-SYSTEM");
    request.SetQueryParameter("ID", "*");
    request.SetQueryParameter("Format", "COMPACT");
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    AssertSuccessfulResponse(httpResponse, getMetadataUrl);
    readUntilEof(httpResponse->GetInputStream(), outputStream);
}

BinaryDataAPtr CLASS::GetMetadata_()
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    BinaryDataAPtr data(new BinaryData());
    
    string getMetadataUrl = mCapabilityUrls->GetGetMetadataUrl();
    RetsHttpRequest request;

    request.SetUrl(getMetadataUrl);
    request.SetMethod(mHttpMethod);
    request.SetQueryParameter("Type", "METADATA-SYSTEM");
    request.SetQueryParameter("ID", "*");
    request.SetQueryParameter("Format", "COMPACT");
    /*
     * Start the transaction, copy to output stream
     */
    RetsHttpResponsePtr httpResponse = DoRequest(&request);
    AssertSuccessfulResponse(httpResponse, getMetadataUrl);

    data->ReadToEof(httpResponse->GetInputStream());
    
    return data;
}

bool CLASS::IsIncrementalMetadata() const
{
    return mIncrementalMetadata;
}

void CLASS::SetIncrementalMetadata(bool incrementalMetadata)
{
    mIncrementalMetadata = incrementalMetadata;
}

void CLASS::SetCollector(MetadataElementCollectorPtr collector)
{
    mLoaderCollector = collector;
}

std::string CLASS::MetadataTypeToString(MetadataElement::Type type)
{
    switch (type)
    {
        case MetadataElement::SYSTEM:
            return "METADATA-SYSTEM";
        case MetadataElement::RESOURCE:
            return "METADATA-RESOURCE";
        case MetadataElement::CLASS:
            return "METADATA-CLASS";
        case MetadataElement::TABLE:
            return "METADATA-TABLE";
        case MetadataElement::UPDATE:
            return "METADATA-UPDATE";
        case MetadataElement::UPDATE_TYPE:
            return "METADATA-UPDATE_TYPE";
        case MetadataElement::OBJECT:
            return "METADATA-OBJECT";
        case MetadataElement::SEARCH_HELP:
            return "METADATA-SEARCH_HELP";
        case MetadataElement::EDIT_MASK:
            return "METADATA-EDIT_MASK";
        case MetadataElement::LOOKUP:
            return "METADATA-LOOKUP";
        case MetadataElement::LOOKUP_TYPE:
            return "METADATA-LOOKUP_TYPE";
        case MetadataElement::UPDATE_HELP:
            return "METADATA-UPDATE_HELP";
        case MetadataElement::VALIDATION_LOOKUP:
            return "METADATA-VALIDATION_LOOKUP";
        case MetadataElement::VALIDATION_LOOKUP_TYPE:
            return "METADATA-VALIDATION_LOOKUP_TYPE";
        case MetadataElement::VALIDATION_EXTERNAL:
            return "METADATA-VALIDATION_EXTERNAL";
        case MetadataElement::VALIDATION_EXTERNAL_TYPE:
            return "METADATA-VALIDATION_EXTERNAL_TYPE";
        case MetadataElement::VALIDATION_EXPRESSION:
            return "METADATA-VALIDATION_EXPRESSION";
        case MetadataElement::FOREIGN_KEY:
            return "METADATA-FOREIGN_KEYS";
        default:
            throw RetsException(str_stream() << "Invalid metadata type: "
                                << type);
    }
}

void CLASS::LoadMetadata(MetadataElement::Type type,
                         std::string level)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    string getMetadataUrl = mCapabilityUrls->GetGetMetadataUrl();
    RetsHttpRequest request;
    request.SetUrl(getMetadataUrl);
    request.SetMethod(mHttpMethod);
    request.SetQueryParameter("Type", MetadataTypeToString(type));
    request.SetQueryParameter("ID", level.empty() ? "0" : level);
    request.SetQueryParameter("Format", "COMPACT");
    RetsHttpResponsePtr httpResponse(DoRequest(&request));

    AssertSuccessfulResponse(httpResponse, getMetadataUrl);
    
    XmlMetadataParserPtr parser(new XmlMetadataParser(mLoaderCollector,
                                                      mErrorHandler));
    parser->SetEncoding(GetEncoding(httpResponse, mEncoding));
    parser->Parse(httpResponse->GetInputStream());
}


void CLASS::InitializeMetadata()
{
    if (mIncrementalMetadata)
    {
        IncrementalMetadataFinderPtr
            incrementalFinder(new IncrementalMetadataFinder(this));
        mMetadata.reset(new RetsMetadata(incrementalFinder));
    }
    else
    {
        RetrieveFullMetadata();
    }
}

void CLASS::RetrieveFullMetadata()
{
    string getMetadataUrl = mCapabilityUrls->GetGetMetadataUrl();
    RetsHttpRequest request;
    request.SetUrl(getMetadataUrl);
    request.SetMethod(mHttpMethod);
    request.SetQueryParameter("Type", "METADATA-SYSTEM");
    request.SetQueryParameter("ID", "*");
    request.SetQueryParameter("Format", "COMPACT");
    RetsHttpResponsePtr httpResponse(DoRequest(&request));

    AssertSuccessfulResponse(httpResponse, getMetadataUrl);
    
    DefaultMetadataCollectorPtr collector(new DefaultMetadataCollector());
    XmlMetadataParserPtr parser(
        new XmlMetadataParser(collector, mErrorHandler));
    parser->SetEncoding(GetEncoding(httpResponse, mEncoding));
    parser->Parse(httpResponse->GetInputStream());

    mMetadata.reset(new RetsMetadata(collector));
}

SearchRequestAPtr CLASS::CreateSearchRequest(string searchType, 
                                             string searchClass,
                                             string query)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    std::auto_ptr<SearchRequest> searchRequest(
        new SearchRequest(searchType, searchClass, query));
    if (mDetectedRetsVersion == RETS_1_0)
    {
        searchRequest->SetQueryType(SearchRequest::DMQL);
    }
    else
    {
        searchRequest->SetQueryType(SearchRequest::DMQL2);
    }
    return searchRequest;
}

SearchResultSetAPtr CLASS::Search(SearchRequest * request)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    string searchUrl = mCapabilityUrls->GetSearchUrl();
    request->SetUrl(searchUrl);
    request->SetMethod(mHttpMethod);
    /*
     * Start the transaction.
     */
    RetsHttpResponsePtr httpResponse = DoRequest(request);

    AssertSuccessfulResponse(httpResponse, searchUrl);

    SearchResultSetAPtr resultSet(new SearchResultSet());
    if (mFlags & MODE_CACHE)
    {
        /*
         * Warning ... must set the caching flag before setting the input stream
         * because that code has the side effecting of initializing the parser and
         * will try to parse the first entry if it exists.
         */
        resultSet->SetCaching(true);
    }
    resultSet->SetEncoding(GetEncoding(httpResponse, mEncoding));
    resultSet->SetInputStream(httpResponse->GetInputStream());
    
    return resultSet;
}

void CLASS::Search(SearchRequest * request, std::ostream & outputStream)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    string searchUrl = mCapabilityUrls->GetSearchUrl();
    request->SetUrl(searchUrl);
    request->SetMethod(mHttpMethod);
    /*
     * Start the transaction, copy to output stream
     */
    RetsHttpResponsePtr httpResponse = DoRequest(request);

    AssertSuccessfulResponse(httpResponse, searchUrl);

    readUntilEof(httpResponse->GetInputStream(), outputStream);
}

BinaryDataAPtr CLASS::Search_(SearchRequest * request)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    BinaryDataAPtr data(new BinaryData());
    
    string searchUrl = mCapabilityUrls->GetSearchUrl();
    request->SetUrl(searchUrl);
    request->SetMethod(mHttpMethod);
    /*
     * Start the transaction, copy to output stream
     */
    RetsHttpResponsePtr httpResponse = DoRequest(request);

    AssertSuccessfulResponse(httpResponse, searchUrl);

    data->ReadToEof(httpResponse->GetInputStream());
    
    return data;
}

istreamPtr CLASS::SearchStream(SearchRequest * request)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    string searchUrl = mCapabilityUrls->GetSearchUrl();
    request->SetUrl(searchUrl);
    request->SetMethod(mHttpMethod);
    /*
     * Start the transaction, copy to output stream
     */
    RetsHttpResponsePtr httpResponse = DoRequest(request);
    
    AssertSuccessfulResponse(httpResponse, searchUrl);

    return httpResponse->GetInputStream();
}

void CLASS::EnableResponseCaching()
{
    mHttpResponseCaching = true;
}

const RetsHttpResponsePtr& CLASS::GetHttpResponse() const
{
    return mHttpResponse;
}

ServerInformationResponseAPtr CLASS::GetServerInformation(std::string resourceName, 
                                                    std::string className, 
                                                    bool standardNames)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");

    ServerInformationRequestAPtr request(new ServerInformationRequest());
    string serverInfoUrl = mCapabilityUrls->GetServerInformationUrl();
    
    if (serverInfoUrl.empty())
        throw RetsException("The ServerInformation Transaction is not supported.");
    
    request->SetClassName(className);
    request->SetResourceName(resourceName);
    request->SetStandardNames(standardNames);
    request->SetUrl(serverInfoUrl);
    request->SetMethod(mHttpMethod);
    
    RetsHttpResponsePtr httpResponse = DoRequest(request.get());

    AssertSuccessfulResponse(httpResponse, serverInfoUrl);

    ServerInformationResponseAPtr serverInformation(new ServerInformationResponse());
    
    serverInformation->SetEncoding(GetEncoding(httpResponse, mEncoding));
    serverInformation->Parse(httpResponse->GetInputStream());

    return serverInformation;    
}

GetObjectResponseAPtr CLASS::GetObject(GetObjectRequest * request)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    RetsHttpRequestPtr httpRequest = request->CreateHttpRequest(mRetsVersion);
    string getObjectUrl = mCapabilityUrls->GetGetObjectUrl();
    httpRequest->SetUrl(getObjectUrl);
    httpRequest->SetMethod(mHttpMethod);
    
    if (mLogEverything)
      httpRequest->SetLogging();
      
    RetsHttpResponsePtr httpResponse = DoRequest(httpRequest.get());

    AssertSuccessfulResponse(httpResponse, getObjectUrl);
    
    GetObjectResponseAPtr response(new GetObjectResponse());
    if (request->HasDefaultObjectKeyAndId())
    {
        response->SetDefaultObjectKeyAndId(request->GetDefaultObjectKey(),
                                           request->GetDefaultObjectId());
    }
    response->SetHttpResponse(httpResponse->GetResponseCode(), httpResponse->GetAdditionalErrorText());
    response->Parse(httpResponse, request->GetIgnoreMalformedHeaders());
    return response;
}

LogoutResponseAPtr CLASS::Logout()
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    mLoggedIn = false;
    LogoutResponseAPtr logoutResponse;
    string logoutUrl = mCapabilityUrls->GetLogoutUrl();
    if (logoutUrl == "")
    {
        return logoutResponse;
    }
    
    RetsHttpRequest request;
    request.SetUrl(logoutUrl);
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
  
    AssertSuccessfulResponse(httpResponse, logoutUrl);

    logoutResponse.reset(new LogoutResponse());
    logoutResponse->Parse(httpResponse->GetInputStream(), mDetectedRetsVersion);
    return logoutResponse;
}

void CLASS::SetUserAgent(string userAgent)
{
    mHttpClient->SetUserAgent(userAgent);
    mUserAgentAuthCalculator.SetUserAgent(userAgent);
}

void CLASS::UseHttpGet(bool useHttpGet)
{
    if (useHttpGet)
    {
        mHttpMethod = RetsHttpRequest::GET;
    }
    else
    {
        mHttpMethod = RetsHttpRequest::POST;
    }
}

void CLASS::SetHttpLogger(RetsHttpLogger * logger)
{
    mHttpClient->SetLogger(logger);
}

RetsHttpLogger* CLASS::GetHttpLogger() const
{
    return mHttpClient->GetLogger();
}

void CLASS::SetHttpLogName(std::string logfile)
{
    if (mLogStream->is_open())
    {
        mLogStream->close();
    }
    
    if (logfile.length() > 0)
    {
        mLogStream->open(logfile.c_str());
        if (mLogStream->is_open())
        {
            mLogger.reset(new StreamHttpLogger(mLogStream.get()));
            SetHttpLogger(mLogger.get());
        }
    }
}

RetsVersion CLASS::GetRetsVersion() const
{
    return mRetsVersion;
}

void CLASS::SetRetsVersion(RetsVersion retsVersion)
{
    mRetsVersion = retsVersion;
}

RetsVersion CLASS::GetDetectedRetsVersion() const
{
    return mDetectedRetsVersion;
}

string CLASS::RetsVersionToString(RetsVersion retsVersion)
{
    if (retsVersion == RETS_1_0)
    {
        return RETS_1_0_STRING;
    }
    else if (retsVersion == RETS_1_5)
    {
        return RETS_1_5_STRING;
    }
    else if (retsVersion == RETS_1_7)
    {
        return RETS_1_7_STRING;
    }
    else if (retsVersion == RETS_1_7_2)
    {
        return RETS_1_7_2_STRING;
    }
    else if (retsVersion == RETS_1_8)
    {
        return RETS_1_8_STRING;
    }
    else if (retsVersion == RETS_1_8_0)
    {
        return RETS_1_8_0_STRING;
    }
    else
    {
        throw RetsException(str_stream() << "Invalid RetsVersion: "
                            << retsVersion);
    }
}

RetsVersion CLASS::RetsVersionFromString(string versionString)
{
    if (versionString == RETS_1_0_STRING)
    {
        return RETS_1_0;
    }
    else if (versionString == RETS_1_5_STRING)
    {
        return RETS_1_5;
    }
    else if (versionString == RETS_1_7_STRING)
    {
        return RETS_1_7;
    }
    else if (versionString == RETS_1_7_2_STRING)
    {
        return RETS_1_7_2;
    }
    else if (versionString == RETS_1_8_STRING)
    {
        return RETS_1_8;
    }
    else if (versionString == RETS_1_8_0_STRING)
    {
        return RETS_1_8_0;
    }
    else if (versionString.empty())
    {
        return RETS_1_0;
    }
    else
    {
        throw RetsException(str_stream() << "Invalid RETS version string: "
                            << versionString);
    }
}

void CLASS::SetErrorHandler(RetsErrorHandler * errorHandler)
{
    mErrorHandler = errorHandler;
}

void CLASS::SetUserAgentAuthType(UserAgentAuthType type)
{
    if (type != USER_AGENT_AUTH_INTEREALTY && type != USER_AGENT_AUTH_RETS_1_7)
    {
        throw RetsException(str_stream()
                            << "Unsupported User-Agent authentication type: "
                            << type);
    }
    
    mUserAgentAuthCalculator.SetUserAgentAuthType(type);
}

UserAgentAuthType CLASS::GetUserAgentAuthType() const
{
    return mUserAgentAuthCalculator.GetUserAgentAuthType();
}

void CLASS::SetUserAgentPassword(std::string userAgentPassword)
{
    mUserAgentAuthCalculator.SetUserAgentPassword(userAgentPassword);
}

void CLASS::SetDefaultEncoding(EncodingType encoding)
{
    mEncoding    = encoding;
}

void CLASS::Cleanup()
{
    mHttpClient.reset();
}

void CLASS::SetLogEverything(bool logging)
{
    mLogEverything = logging;
}

void CLASS::SetModeFlags(unsigned int flags)
{
    mFlags = flags;
    
    /*
     * If we are in no streaming mode, the cache is required.
     */
    if (mFlags & MODE_NO_STREAM)
        mFlags |= MODE_CACHE;
        
    /*
     * If we are disabling SSL verification, tell http client.
     */
    if (mFlags & MODE_NO_SSL_VERIFY)
        mHttpClient->SetModeFlags(MODE_NO_SSL_VERIFY);
}


void CLASS::SetProxy(std::string url, std::string password)
{
    mHttpClient->SetProxy(url,password);
}

void CLASS::SetTimeout(int seconds)
{
    mTimeout = seconds;
    mHttpClient->SetTimeout(mTimeout);
}

string CLASS::GetLibraryVersion()
{
    return LIBRETS_VERSION;
}

UpdateRequestAPtr CLASS::CreateUpdateRequest(string resourceName,
                                             string className)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    std::auto_ptr<UpdateRequest> updateRequest(
        new UpdateRequest(resourceName, className));
    return updateRequest;
}

UpdateResponseAPtr CLASS::Update(UpdateRequest * request)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
        
    string updateUrl = mCapabilityUrls->GetUpdateUrl();
    request->SetUrl(updateUrl);
    request->SetMethod(RetsHttpRequest::POST);
    request->ConstructRequest();
    /*
     * Start the transaction.
     */
    RetsHttpResponsePtr httpResponse = DoRequest(request);

    AssertSuccessfulResponse(httpResponse, updateUrl);

    UpdateResponseAPtr result(new UpdateResponse());
    result->SetEncoding(GetEncoding(httpResponse, mEncoding));
    result->SetInputStream(httpResponse->GetInputStream());
    
    return result;
}

PayloadListResultSetAPtr CLASS::GetPayloadList(std::string metadataID)
{
    if (!mLoggedIn)
        throw RetsException("You are not logged in");
    
    PayloadListResultSetAPtr payloadListResultSet;
    string payloadListUrl = mCapabilityUrls->GetPayloadListUrl();
    if (payloadListUrl == "")
    {
        return payloadListResultSet;
    }
    
    RetsHttpRequest request;
    request.SetUrl(payloadListUrl);
    if (!metadataID.empty())
    {
        request.SetQueryParameter("ID", metadataID);
    }
    RetsHttpResponsePtr httpResponse(DoRequest(&request));
    
    AssertSuccessfulResponse(httpResponse, payloadListUrl);
    
    payloadListResultSet.reset(new PayloadListResultSet());
    payloadListResultSet->SetEncoding(GetEncoding(httpResponse, mEncoding));
    payloadListResultSet->Parse(httpResponse->GetInputStream());
    return payloadListResultSet;
}



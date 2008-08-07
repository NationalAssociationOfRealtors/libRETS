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
#ifndef LIBRETS_RETS_HTTP_CLIENT_H
#define LIBRETS_RETS_HTTP_CLIENT_H
/** 
 * @file RetsHttpClient.h
 * (Internal) Contains the Curl Http Client interface prototype for use with libCURL.
 */
/// @cond MAINTAINER

#include <string>
#include "librets/http_forward.h"

namespace librets {
/**
 * (Internal) RetsHttpClient is the super class defining the activities that a client may perform.
 */
class RetsHttpClient
{
  public:
    static RetsHttpClientPtr CreateDefault();

    virtual ~RetsHttpClient();
    
    virtual void SetUserCredentials(std::string userName, std::string password)
        = 0;

    virtual void SetDefaultHeader(std::string name, std::string value) = 0;
    
    virtual std::string GetDefaultHeader(std::string name) const = 0;
   
    virtual void ClearDefaultHeader(std::string name) =  0;
    
    virtual void SetUserAgent(std::string userAgent) = 0;
    
    virtual std::string GetUserAgent() const = 0;

    virtual RetsHttpResponsePtr StartRequest(RetsHttpRequest * request) = 0;
    
    virtual bool ContinueRequest() = 0;
    
    virtual int GetResponseCode() = 0;

    virtual void SetLogger(RetsHttpLogger * logger) = 0;

    virtual RetsHttpLogger* GetLogger() const = 0;
    
    virtual std::string GetCookie(const char * name) = 0;
	
	virtual void SetProxy(std::string url, std::string password) = 0;
};

};
/// @endcond

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

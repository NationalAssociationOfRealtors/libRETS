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
#ifndef LIBRETS_RETS_HTTP_REQUEST_H
#define LIBRETS_RETS_HTTP_REQUEST_H
/** 
 * @file RetsHttpRequest.h
 * (Internal) Contains the Curl Http Request interface class for use with libCURL.
 */
/// @cond MAINTAINER

#include <string>
#include <map>

#include "librets/RetsObject.h"
#include "librets/http_forward.h"

namespace librets {

typedef std::map<std::string, std::string> StringMap;

/**
 * (Internal) RetsHttpRequest is a class that defines the http client requests that can be made
 * through libcurl.
 */
class RetsHttpRequest : public virtual RetsObject
{
  public:
    enum Method
    {
        GET,
        POST
    };

    RetsHttpRequest();
    
    virtual ~RetsHttpRequest();

    virtual Method GetMethod() const;

    virtual void SetMethod(Method method);

    virtual std::string GetUrl() const;

    virtual void SetUrl(std::string url);

    virtual void SetHeader(std::string name, std::string value);
    
    virtual void ClearHeader(std::string name);
    
    StringMap GetHeaderMap() const;

    virtual void SetQueryParameter(std::string, std::string value);
    
    virtual void SetQueryParameter(std::string, int value);

    virtual std::string GetQueryString() const;

    bool GetLogging();
    
    void SetNoLogging();
    
    void SetLogging();
    
  private:
    void GenerateQueryString() const;
    
    Method mMethod;
    std::string mUrl;
    StringMap mQueryParameters;
    mutable bool mQueryParametersChanged;
    mutable std::string mQueryString;
    StringMap mHeaders;
    bool mLogging;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

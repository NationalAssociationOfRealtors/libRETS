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

#ifndef LIBRETS_RETS_HTTP_RESPONSE_H
#define LIBRETS_RETS_HTTP_RESPONSE_H
/** 
 * @file RetsHttpResponse.h
 * (Internal) Contains the Curl Http response interface class for use with libCURL.
 */
/// @cond MAINTAINER

#include <string>
#include <istream>
#include "librets/std_forward.h"

namespace librets {

/**
 * (Internal) RetsHttpResponse is the super class that defines those operations to be
 * returned as the result of an http transaction by libcurl.
 */
class RetsHttpResponse
{
  public:
    virtual ~RetsHttpResponse();

    /**
     * Return a header value.
     *
     * @return Header value
     * @throw RetsException
     */
    virtual std::string GetHeader(std::string name) const = 0;

    /**
     * This provides access to the HTTP response headers.
     * @return A const reference to the header map.
     */
    virtual const StringMap& GetHeaders() const = 0;
    
    std::string GetContentType() const;

    /**
     * Return an input stream to the data.
     *
     * @return Input stream
     * @throw RetsException
     */
    virtual istreamPtr GetInputStream() const = 0;

    /**
     * Return the HTTP response code.
     *
     * @return HTTP response code
     * @throw RetsException
     */
    virtual int GetResponseCode() const = 0;
    
    /**
     * Set the HTTP response code.
     *
     * @param responseCode An integer representing the http response code for this transaction.
     */
    virtual void SetResponseCode(int responseCode) = 0;
    
    /**
     * Return any extended error text.
     *
     * @return A string containing additional error information.
     */
    virtual std::string GetAdditionalErrorText() = 0;
    
    /**
     * Set any extended error text.
     *
     * @param errorText A string with additional error text.
     */
    virtual void SetAdditionalErrorText(std::string errorText) = 0;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

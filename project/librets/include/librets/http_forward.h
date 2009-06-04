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
#ifndef LIBRETS_HTTP_FORWARD_H
#define LIBRETS_HTTP_FORWARD_H
/** 
 * @file http_forward.h
 * (Internal) Contains the Curl Http forward declarations for use with libCURL.
 */
/// @cond MAINTAINER

#include <boost/shared_ptr.hpp>
#include <vector>

namespace librets {

class RetsHttpClient;
/** Smart pointer to RetsHttpClient. */
typedef boost::shared_ptr<RetsHttpClient> RetsHttpClientPtr;

class RetsHttpRequest;
/** Smart pointer to RetsHttpRequest. */
typedef boost::shared_ptr<RetsHttpRequest> RetsHttpRequestPtr;

class RetsHttpResponse;
/** Smart pointer to RetsHttpResponse. */
typedef boost::shared_ptr<RetsHttpResponse> RetsHttpResponsePtr;

class RetsHttpLogger;
/** Smart pointer to RetsHttpLogger. */
typedef boost::shared_ptr<RetsHttpLogger> RetsHttpLoggerPtr;

class NullHttpLogger;

class StreamHttpLogger;

class CurlEasy;
/** Smart pointer to CurlEasy. */
typedef boost::shared_ptr<CurlEasy> CurlEasyPtr;
/** A vector of CurlEasy objects. */
typedef std::vector<CurlEasyPtr> CurlEasyVector;
/** Smart pointer to CurlEasyVector. */
typedef boost::shared_ptr<CurlEasyVector> CurlEasyVectorPtr;

class CurlHttpClient;
/** Smart pointer to CurlHttpClient. */
typedef boost::shared_ptr<CurlHttpClient> CurlHttpClientPtr;

class CurlHttpRequest;
/** Smart pointer to CurlHttpRequest. */
typedef boost::shared_ptr<CurlHttpRequest> CurlHttpRequestPtr;

class CurlHttpResponse;
/** Smart pointer to CurlHttpResponse. */
typedef boost::shared_ptr<CurlHttpResponse> CurlHttpResponsePtr;

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

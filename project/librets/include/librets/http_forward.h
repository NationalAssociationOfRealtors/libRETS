#ifndef LIBRETS_HTTP_FORWARD_H
#define LIBRETS_HTTP_FORWARD_H

#include <boost/shared_ptr.hpp>

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

class CurlHttpClient;
typedef boost::shared_ptr<CurlHttpClient> CurlHttpClientPtr;

class CurlHttpRequest;
typedef boost::shared_ptr<CurlHttpRequest> CurlHttpRequestPtr;

class CurlHttpResponse;
typedef boost::shared_ptr<CurlHttpResponse> CurlHttpResponsePtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

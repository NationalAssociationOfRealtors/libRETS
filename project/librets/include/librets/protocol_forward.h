#ifndef LIBRETS_PROTOCOL_FORWARD_H
#define LIBRETS_PROTOCOL_FORWARD_H

#include <boost/shared_ptr.hpp>

namespace librets {

class RetsSession;
/** Smart pointer to RetsSession. */
typedef boost::shared_ptr<RetsSession> RetsSessionPtr;

class LoginResponse;
/** Smart pointer to LoginResponse. */
typedef boost::shared_ptr<LoginResponse> LoginResponsePtr;

class CapabilityUrls;
/** Smart pointer to CapabilityUrls. */
typedef boost::shared_ptr<CapabilityUrls> CapabilityUrlsPtr;

class SearchRequest;
/** Smart pointer to SearchRequest. */
typedef boost::shared_ptr<SearchRequest> SearchRequestPtr;

class SearchResultSet;
/** Smart pointer to SearchResultSet. */
typedef boost::shared_ptr<SearchResultSet> SearchResultSetPtr;

class GetObjectRequest;
/** Smart pointer to GetObjectRequest. */
typedef boost::shared_ptr<GetObjectRequest> GetObjectRequestPtr;

class ObjectDescriptor;
/** Smart pointer to ObjectDescriptor. */
typedef boost::shared_ptr<ObjectDescriptor> ObjectDescriptorPtr;

class GetObjectResponse;
/** Smart pointer to GetObjectResponse. */
typedef boost::shared_ptr<GetObjectResponse> GetObjectResponsePtr;

class LogoutResponse;
/** Smart pointer to LogoutResponse. */
typedef boost::shared_ptr<LogoutResponse> LogoutResponsePtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

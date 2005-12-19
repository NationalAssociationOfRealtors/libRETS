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
typedef std::auto_ptr<SearchRequest> SearchRequestAPtr;

class SearchResultSet;
/** Smart pointer to SearchResultSet. */
typedef boost::shared_ptr<SearchResultSet> SearchResultSetPtr;
typedef std::auto_ptr<SearchResultSet> SearchResultSetAPtr;

class GetObjectRequest;
/** Smart pointer to GetObjectRequest. */
typedef boost::shared_ptr<GetObjectRequest> GetObjectRequestPtr;
typedef std::auto_ptr<GetObjectRequest> GetObjectRequestAPtr;

class ObjectDescriptor;
/** Smart pointer to ObjectDescriptor. */
typedef boost::shared_ptr<ObjectDescriptor> ObjectDescriptorPtr;
typedef std::auto_ptr<ObjectDescriptor> ObjectDescriptorAPtr;

class GetObjectResponse;
/** Smart pointer to GetObjectResponse. */
typedef boost::shared_ptr<GetObjectResponse> GetObjectResponsePtr;
typedef std::auto_ptr<GetObjectResponse> GetObjectResponseAPtr;

class LogoutResponse;
/** Smart pointer to LogoutResponse. */
typedef boost::shared_ptr<LogoutResponse> LogoutResponsePtr;
typedef std::auto_ptr<LogoutResponse> LogoutResponseAPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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

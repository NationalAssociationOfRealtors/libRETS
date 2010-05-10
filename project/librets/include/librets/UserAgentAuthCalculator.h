/*
 * Copyright (C) 2006 National Association of REALTORS(R)
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

#ifndef LIBRETS_RETS_USER_AGENT_AUTH_CALCULATOR_H
#define LIBRETS_RETS_USER_AGENT_AUTH_CALCULATOR_H
/** 
 * @file UserAgentAuthCalculator.h
 * Contains the UserAgentAuthCalculator class definition.
 */
/// @cond MAINTAINER

#include <string>
#include "librets/RetsObject.h"
#include "librets/UserAgentAuthType.h"

namespace librets {

/**
 * (Internal) UserAgentAuthCalculator is an internal support class
 * used during the calculation of User Agent Authentication value.
 */    
class UserAgentAuthCalculator : public RetsObject
{
  public:
    
    UserAgentAuthCalculator() : mUserAgentAuthType(USER_AGENT_AUTH_RETS_1_7) {};
    
    /**
     * Get the User Agent authorization type currently set.
     * @return The <code>UserAgentAuthType</code>
     */
    UserAgentAuthType GetUserAgentAuthType() const;
    
    /**
     * Set the User Agent name
     * @param userAgent A string containing the User Agent Name.
     */
    void SetUserAgent(std::string userAgent);
    
    /**
     * Set the User Agent authorization type
     * @param userAgentAuthType A <code>UserAgentAuthType</code> method.
     */
    void SetUserAgentAuthType(UserAgentAuthType userAgentAuthType);
    
    /**
     * Set the Uesr Agent Password
     * @param userAgentPassword A string containing the User Agent Password.
     */
    void SetUserAgentPassword(std::string userAgentPassword);
    
    /**
     * Set the Requst Id.
     * @param requestId A string containing the Request ID from the http header.
     */
    void SetRequestId(std::string requestId);
    
    /**
     * Set the Session Id.
     * @param sessionId A string containing the Session ID from the http header.
     */
    void SetSessionId(std::string sessionId);
    
    /**
     * Set the version information.
     * @param versionInfo A string containing the version information.
     */
    void SetVersionInfo(std::string versionInfo);
    
    /**
     * Indicate whether or not an authorization value has been calculated.
     * @return A boolean that if <code>true</code> indicates a valid authorization value 
     * has been calculated.
     */
    bool HasAuthorizationValue() const;
    
    /**
     * Returns the Authorization Value
     * @return The calculated Authorization Value as a string.
     */
    std::string AuthorizationValue() const;;
    
  private:
    std::string         mUserAgent;
    UserAgentAuthType   mUserAgentAuthType;
    std::string         mUserAgentPassword;
    std::string         mRequestId;
    std::string         mSessionId;
    std::string         mVersionInfo;
};
    
}
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

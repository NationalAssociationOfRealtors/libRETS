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

#include <string>
#include "librets/RetsObject.h"

namespace librets {
    
class UserAgentAuthCalculator : public RetsObject
{
  public:
    void SetUserAgent(std::string userAgent);
    void SetUserAgentPassword(std::string userAgentPassword);
    void SetRequestId(std::string requestId);
    void SetSessionId(std::string sessionId);
    void SetVersionInfo(std::string versionInfo);
    
    bool HasAuthorizationValue() const;
    std::string AuthorizationValue() const;;
    
  private:
    std::string mUserAgent;
    std::string mUserAgentPassword;
    std::string mRequestId;
    std::string mSessionId;
    std::string mVersionInfo;
};
    
}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

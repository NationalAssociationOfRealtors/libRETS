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

#ifndef LIBRETS_LOGOUT_RESPONSE_H
#define LIBRETS_LOGOUT_RESPONSE_H

#include <string>
#include <boost/shared_ptr.hpp>
#include "librets/std_forward.h"
#include "librets/RetsObject.h"
#include "librets/RetsVersion.h"

namespace librets {

class LogoutResponseImpl;
/** Smart pointer to LogoutResponseImpl. */
typedef boost::shared_ptr<LogoutResponseImpl> LogoutResponseImplPtr;
    
/**
 *  Logout information.
 */
class LogoutResponse : public RetsObject
{
  public:
    LogoutResponse();

    void Parse(istreamPtr inputStream, RetsVersion retsVersion);
    
    std::string GetValue(std::string key) const;
    
    bool ReceivedResponse() const;
    
    /**
     * Returns billing information given by the server.
     *
     * @return Billing informaiton
     */
    std::string GetBillingInfo() const;

    /**
     * Returns the logout message from the server.
     *
     * @return logout message
     */
    std::string GetLogoutMessage() const;

    /**
     * Returns the number of seconds connected.
     *
     * @return The number of seconds connected
     */
    int GetConnectTime() const;

  private:
    LogoutResponseImplPtr mImpl;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

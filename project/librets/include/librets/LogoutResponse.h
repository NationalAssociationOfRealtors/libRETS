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
/**
 * @file LogoutResponse.h 
 * Contains the LogoutResponse class definition.
 */

#include <string>

#include "librets/KeyValueResponse.h"

namespace librets {

/**
 * LogoutResponse contains the API that allows access to the information
 * sent in response to a successful RETS LOGOUT transaction.
 */
class LogoutResponse : public KeyValueResponse
{
  public:
    /**
     * Returns billing information given by the server.
     *
     * @return A string containing the Billing informaiton
     */
    std::string GetBillingInfo() const;

    /**
     * Returns the logout message from the server.
     *
     * @return A string containing the logout message
     */
    std::string GetLogoutMessage() const;

    /**
     * Returns the number of seconds connected.
     *
     * @return An integer representing the number of seconds connected
     */
    int GetConnectTime() const;

  private:
    virtual void ParsingFinished();
    
    virtual RetsXmlTextEventPtr GetBodyEvent(RetsXmlEventListPtr eventList,
                                             RetsVersion retsVersion);

    RetsXmlTextEventPtr GetBodyEventFromEmptyLogoutResponse(
        RetsXmlEventListPtr eventList);
    
    int mConnectTime;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

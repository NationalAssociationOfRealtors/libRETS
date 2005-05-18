#ifndef LIBRETS_LOGOUT_RESPONSE_H
#define LIBRETS_LOGOUT_RESPONSE_H

#include <string>

#include "librets/KeyValueResponse.h"

namespace librets {

/**
 *  Logout information.
 */
class LogoutResponse : public KeyValueResponse
{
  public:
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
    virtual void ParsingFinished();

    int mConnectTime;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

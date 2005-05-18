#ifndef LIBRETS_LOGOUT_RESPONSE_H
#define LIBRETS_LOGOUT_RESPONSE_H

#include <string>

namespace librets {

/**
 *  Logout information.
 */
class LogoutResponse
{
  public:

    /**
     * Returns billing information given by the server.
     *
     * @return billing informaiton
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
     * @return the number of seconds connected
     */
    int GetSeconds() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

#ifndef LIBRETS_RETS_SESSION_H
#define LIBRETS_RETS_SESSION_H

#include <string>
#include "librets/forward.h"

/**
 * The main librets namespace.  See RetsSession.
 */
namespace librets {

/**
 * Interface to RETS session.
 */
class RetsSession
{
  public:
    /**
     * Create a new RETS session with a login URL.
     *
     * @param loginUrl The login URL to a RETS server
     * @throws RetsException if an error occurs.
     */
    RetsSession(std::string loginUrl);

    /**
     * Logs into the RETS server.
     *
     * @param userName user name
     * @param password password
     * @return True, if login is successful.
     * @throws RetsException if an error occurs.
     */
    bool Login(std::string userName, std::string password);

    /**
     * Performs a search on the server.
     *
     * @param request search request parameters
     * @return Search result set
     * @throws RetsException if an error occurs.
     */
    SearchResultSetPtr Search(const SearchRequest & request);

    /**
     * Returns the metadata for this server.  Only valid after logging
     * in.
     *
     * @return The metadata for this server
     * @throws RetsException if an error occurs.
     */
    RetsMetadataPtr GetMetadata() const;

    /**
     * Logs out of the server.
     *
     * @return Logout information
     * @throws RetsException if an error occurs.
     */
    LogoutResponsePtr Logout();
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

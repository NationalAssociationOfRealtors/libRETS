#ifndef LIBRETS_RETS_SESSION_H
#define LIBRETS_RETS_SESSION_H

#include <string>
#include "librets/protocol_forward.h"
#include "librets/http_forward.h"

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
     * Returns the contents of tne action URL from the last successful
     * login.
     *
     * @return the contents of the action URL
     * @throws RetsException if an error occurs.
     */
    std::string GetAction();

    /**
     * Performs a search on the server.
     *
     * @param request search request parameters
     * @return Search result set
     * @throws RetsException if an error occurs.
     */
    SearchResultSetPtr Search(SearchRequestPtr request);

    /**
     * Returns the metadata for this server.  Only valid after logging
     * in.
     *
     * @return The metadata for this server
     * @throws RetsException if an error occurs.
     */
    RetsMetadataPtr GetMetadata();
    
    GetObjectResponsePtr GetObject(GetObjectRequestPtr request);

    /**
     * Logs out of the server.
     *
     * @return Logout information
     * @throws RetsException if an error occurs.
     */
    LogoutResponsePtr Logout();

  private:
    void RetrieveAction();

    void RetrieveMetadata();

    void AssertSuccessfulResponse(RetsHttpResponsePtr response,
                                  std::string url);

    RetsHttpClientPtr mHttpClient;

    std::string mLoginUrl;

    CapabilityUrlsPtr mCapabilityUrls;

    std::string mAction;

    RetsMetadataPtr mMetadata;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

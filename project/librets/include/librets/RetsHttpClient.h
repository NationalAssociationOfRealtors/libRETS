#ifndef LIBRETS_RETS_HTTP_CLIENT_H
#define LIBRETS_RETS_HTTP_CLIENT_H

#include <string>
#include "librets/http_forward.h"

namespace librets {

class RetsHttpClient
{
  public:
    static RetsHttpClientPtr CreateDefault();

    virtual ~RetsHttpClient();
    
    virtual void SetUserCredentials(std::string userName, std::string password)
        = 0;

    virtual void AddDefaultHeader(std::string name, std::string value) = 0;

    virtual RetsHttpResponsePtr DoRequest(RetsHttpRequestPtr request) = 0;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

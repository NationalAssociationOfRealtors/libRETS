#ifndef LIBRETS_CURL_HTTP_CLIENT_H
#define LIBRETS_CURL_HTTP_CLIENT_H

#include <vector>
#include <sstream>
#include <istream>
#include "librets/curl.h"
#include "librets/RetsHttpClient.h"
#include "librets/CurlSlist.h"
#include "librets/RetsException.h"

namespace librets {

typedef boost::shared_ptr<std::stringstream> stringstreamPtr;

class CurlHttpClient : public RetsHttpClient
{
  public:
    CurlHttpClient();
    
    virtual ~CurlHttpClient();

    virtual void SetUserCredentials(std::string userName,
                                    std::string password);

    virtual void AddDefaultHeader(std::string name, std::string value);

    virtual RetsHttpResponsePtr DoRequest(RetsHttpRequestPtr request);

  private:
    void CurlAssert(const RetsExceptionContext & context, CURLcode errorCode,
                    bool useErrorBuffer = true);
    static size_t WriteData(void * buffer, size_t size, size_t nmemb,
                            void * userData);
    static size_t WriteHeader(void * buffer, size_t size, size_t nmemb,
                              void * userData);

    CURL * mCurl;
    char * mCurlErrorBuffer[CURL_ERROR_SIZE];
    CurlSlist mHeaders;
    CurlHttpResponsePtr mResponse;

    // Need to keep copies of strings we pass to curl, as it just stores
    // a pointer (i.e. it does not copy the contents into the library)
    std::string mCurlUserpwd;
    std::string mCurlUrl;
    std::string mQueryString;
    std::vector<std::string> mDefaultHeaderStrings;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

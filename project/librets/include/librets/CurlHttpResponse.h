#ifndef LIBRETS_CURL_HTTP_RESPONSE_H
#define LIBRETS_CURL_HTTP_RESPONSE_H

#include "librets/RetsHttpResponse.h"
#include "librets/std_forward.h"

namespace librets {

class CurlHttpResponse : public RetsHttpResponse
{
  public:
    CurlHttpResponse();

    virtual ~CurlHttpResponse();

    void SetUrl(std::string url);

    virtual std::string GetHeader(std::string name) const;
    
    void SetHeader(std::string name, std::string value);

    virtual istreamPtr GetInputStream() const;
    
    void SetStream(iostreamPtr inputStream);
    
    void WriteData(const char * data, size_t bytes);
    
    virtual int GetResponseCode() const;

    void SetResponseCode(int responseCode);

  private:
    iostreamPtr mStream;
    int mResponseCode;
    StringMap mHeaders;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

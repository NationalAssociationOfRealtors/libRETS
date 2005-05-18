#ifndef LIBRETS_RETS_HTTP_REQUEST_H
#define LIBRETS_RETS_HTTP_REQUEST_H

#include <string>
#include <map>

#include "librets/RetsObject.h"

namespace librets {

typedef std::map<std::string, std::string> StringMap;

class RetsHttpRequest : public virtual RetsObject
{
  public:
    enum Method
    {
        GET,
        POST
    };

    RetsHttpRequest();
    
    virtual ~RetsHttpRequest();

    virtual Method GetMethod() const;

    virtual void SetMethod(Method method);

    virtual std::string GetUrl() const;

    virtual void SetUrl(std::string url);

    virtual void SetHeader(std::string name, std::string value);

    virtual void SetQueryParameter(std::string, std::string value);

    virtual std::string GetQueryString() const;

  private:
    Method mMethod;
    std::string mUrl;
    StringMap mQueryParameters;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

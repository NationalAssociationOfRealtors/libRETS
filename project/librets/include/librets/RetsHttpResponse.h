#ifndef LIBRETS_RETS_HTTP_RESPONSE_H
#define LIBRETS_RETS_HTTP_RESPONSE_H

#include <string>
#include <istream>
#include "boost/shared_ptr.hpp"

namespace librets {

typedef boost::shared_ptr<std::istream> istreamPtr;

/**
 * HTTP response expected for RETS.
 */
class RetsHttpResponse
{
  public:
    /**
     * Virtual destructor.
     */
    virtual ~RetsHttpResponse();

    /**
     * Return a header value.
     *
     * @return Header value
     * @throw RetsException
     */
    virtual std::string GetHeader(std::string name) const = 0;
    
    std::string GetContentType() const;

    /**
     * Return an input stream to the data.
     *
     * @return Input stream
     * @throw RetsException
     */
    virtual istreamPtr GetInputStream() const = 0;

    /**
     * Return the HTTP response code.
     *
     * @return HTTP response code
     * @throw RetsException
     */
    virtual int GetResponseCode() const = 0;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

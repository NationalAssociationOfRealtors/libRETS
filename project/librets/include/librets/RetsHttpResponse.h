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

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

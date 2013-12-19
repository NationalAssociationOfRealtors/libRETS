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
#ifndef LIBRETS_TEST_HTTP_RESPONSE_H
#define LIBRETS_TEST_HTTP_RESPONSE_H

#include "librets/std_forward.h"
#include "librets/RetsHttpResponse.h"

namespace librets {
    
class TestHttpResponse : public RetsHttpResponse
{
  public:
    TestHttpResponse(std::string resourceName);
    
    virtual ~TestHttpResponse();
    
    virtual std::string GetHeader(std::string name) const;
    
    virtual const StringMultiMap& GetHeaders() const;
    
    virtual istreamPtr GetInputStream() const;
    
    virtual int GetResponseCode() const;
    
    virtual void SetResponseCode(int responseCode);
    
    virtual std::string GetAdditionalErrorText();
    
    virtual void SetAdditionalErrorText(std::string errorText);
    
  private:
    StringMultiMap mHeaders;
    int mResponseCode;
    istreamPtr mInputStream;

};

}

#endif

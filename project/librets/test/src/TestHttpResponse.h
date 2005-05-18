#ifndef LIBRETS_TEST_HTTP_RESPONSE_H
#define LIBRETS_TEST_HTTP_RESPONSE_H

/*
 *  TestHttpResponse.h
 *  librets
 *
 *  Created by Dave Dribin on 4/5/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "librets/std_forward.h"
#include "librets/RetsHttpResponse.h"

namespace librets {
    
class TestHttpResponse : public RetsHttpResponse
{
  public:
    TestHttpResponse(std::string resourceName);
    
    virtual ~TestHttpResponse();
    
    virtual std::string GetHeader(std::string name) const;
    
    virtual istreamPtr GetInputStream() const;
    
    virtual int GetResponseCode() const;
    
  private:
    StringMap mHeaders;
    int mResponseCode;
    istreamPtr mInputStream;

};

}

#endif
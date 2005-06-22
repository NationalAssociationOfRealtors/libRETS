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
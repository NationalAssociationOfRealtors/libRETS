/*
 *  TestHttpResponseTest.cpp
 *  librets
 *
 *  Created by Dave Dribin on 4/5/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "TestHttpResponse.h"
#include "librets/util.h"
#include "librets/http_forward.h"

using namespace librets;
using namespace librets::util;
using namespace std;

#define CLASS TestHttpResponseTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testResponse);
    CPPUNIT_TEST_SUITE_END();
    
protected:
    void testResponse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testResponse()
{
    RetsHttpResponsePtr response(new TestHttpResponse("test-response.txt"));
    ASSERT_EQUAL(200, response->GetResponseCode());
    ASSERT_STRING_EQUAL("text/plain", response->GetHeader("Content-Type"));
    ASSERT_STRING_EQUAL("header:with:colons",
                        response->GetHeader("Colon-Header"));
    string content = readIntoString(*response->GetInputStream());
    ASSERT_STRING_EQUAL("this is\nsample text.\n", content);
}
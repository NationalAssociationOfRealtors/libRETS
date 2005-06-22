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
    ASSERT_STRING_EQUAL("this is\r\nsample text.\r\n", content);
}

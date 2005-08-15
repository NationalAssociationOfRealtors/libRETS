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
#include <sstream>
#include "testUtil.h"
#include "librets/RetsHttpRequest.h"

using namespace librets;
using namespace std;

#define CLASS RetsHttpRequestTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testQueryStringNoParameters);
    CPPUNIT_TEST(testQueryStringSingleParameter);
    CPPUNIT_TEST(testQueryStringIntParameter);
    CPPUNIT_TEST(testQueryStringMultipleParameters);
    CPPUNIT_TEST(testQueryStringRemoveParameter);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testQueryStringNoParameters();
    void testQueryStringSingleParameter();
    void testQueryStringIntParameter();
    void testQueryStringMultipleParameters();
    void testQueryStringRemoveParameter();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testQueryStringNoParameters()
{
    RetsHttpRequest request;
    ASSERT_STRING_EQUAL("", request.GetQueryString());
}

void CLASS::testQueryStringSingleParameter()
{
    RetsHttpRequest request;
    request.SetQueryParameter("name", "value");
    ASSERT_STRING_EQUAL("name=value", request.GetQueryString());
}

void CLASS::testQueryStringIntParameter()
{
    RetsHttpRequest request;
    request.SetQueryParameter("name", 42);
    ASSERT_STRING_EQUAL("name=42", request.GetQueryString());
}

void CLASS::testQueryStringMultipleParameters()
{
    RetsHttpRequest request;
    request.SetQueryParameter("name", "value");
    request.SetQueryParameter("foo", "bar");
    ASSERT_STRING_EQUAL("foo=bar&name=value", request.GetQueryString());
}

void CLASS::testQueryStringRemoveParameter()
{
    RetsHttpRequest request;
    request.SetQueryParameter("name", "value");
    request.SetQueryParameter("foo", "bar");
    // This should remove name
    request.SetQueryParameter("name", "");
    ASSERT_STRING_EQUAL("foo=bar", request.GetQueryString());
}

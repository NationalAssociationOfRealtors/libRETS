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
#include "librets/LogoutResponse.h"

using namespace librets;
using namespace std;

#define CLASS LogoutResponseTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testValidResponse);
    CPPUNIT_TEST(testShortResponse);
    CPPUNIT_TEST(testTarasoftResponse);
    CPPUNIT_TEST(testCrisResponse);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testValidResponse();
    void testShortResponse();
    void testTarasoftResponse();
    void testCrisResponse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testValidResponse()
{
    istreamPtr inputStream = getResource("logout-valid15.xml");
    LogoutResponse response;
    response.Parse(inputStream, RETS_1_5);

    ASSERT_STRING_EQUAL("51", response.GetValue("ConnectTime"));
    ASSERT_STRING_EQUAL("51", response.GetValue("connecttime"));
    ASSERT_STRING_EQUAL("", response.GetValue("none"));
    
    ASSERT_STRING_EQUAL("0.36", response.GetBillingInfo());
    ASSERT_STRING_EQUAL("Goodbye", response.GetLogoutMessage());
    ASSERT_EQUAL(51, response.GetConnectTime());
}

void CLASS::testShortResponse()
{
    istreamPtr inputStream = getResource("short-success-response.xml");
    LogoutResponse response;
    response.Parse(inputStream, RETS_1_5);
    ASSERT_STRING_EQUAL("", response.GetBillingInfo());
    ASSERT_STRING_EQUAL("", response.GetLogoutMessage());
    ASSERT_EQUAL(0, response.GetConnectTime());
}

void CLASS::testTarasoftResponse()
{
    istreamPtr inputStream = getResource("logout-tarasoft.xml");
    LogoutResponse response;
    response.Parse(inputStream, RETS_1_5);
    ASSERT_STRING_EQUAL("", response.GetBillingInfo());
    ASSERT_STRING_EQUAL("", response.GetLogoutMessage());
    ASSERT_EQUAL(0, response.GetConnectTime());
}

void CLASS::testCrisResponse()
{
    istreamPtr inputStream = getResource("logout-cris.xml");
    LogoutResponse response;
    response.Parse(inputStream, RETS_1_5);
    ASSERT_STRING_EQUAL("", response.GetBillingInfo());
    ASSERT_STRING_EQUAL("", response.GetLogoutMessage());
    ASSERT_EQUAL(0, response.GetConnectTime());
}

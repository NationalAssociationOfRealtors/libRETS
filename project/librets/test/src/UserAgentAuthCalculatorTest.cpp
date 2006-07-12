/*
 * Copyright (C) 2006 National Association of REALTORS(R)
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
#include <stdexcept>
#include "testUtil.h"
#include "librets/UserAgentAuthCalculator.h"

using namespace librets;
using namespace std;

#define CLASS UserAgentAuthCalculatorTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testAuthorizationValue);
    CPPUNIT_TEST(testNoPassword);
    CPPUNIT_TEST_SUITE_END();
    
protected:
    void testAuthorizationValue();
    void testNoPassword();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testAuthorizationValue()
{
    UserAgentAuthCalculator authCalc;
    authCalc.SetUserAgent("VieleRETS/1.1.0");
    authCalc.SetUserAgentPassword("NWA");
    authCalc.SetRequestId("");
    authCalc.SetSessionId("");
    authCalc.SetVersionInfo("RETS/1.5");
    
    CPPUNIT_ASSERT(authCalc.HasAuthorizationValue());
    ASSERT_STRING_EQUAL("07554bfdcfb2b37e280b6d9c4826bc46",
                        authCalc.AuthorizationValue());
}

void CLASS::testNoPassword()
{
    UserAgentAuthCalculator authCalc;
    authCalc.SetUserAgent("VieleRETS/1.1.0");
    authCalc.SetUserAgentPassword("");
    authCalc.SetRequestId("");
    authCalc.SetSessionId("");
    authCalc.SetVersionInfo("RETS/1.5");
    
    CPPUNIT_ASSERT(!authCalc.HasAuthorizationValue());
    ASSERT_STRING_EQUAL("", authCalc.AuthorizationValue());
}

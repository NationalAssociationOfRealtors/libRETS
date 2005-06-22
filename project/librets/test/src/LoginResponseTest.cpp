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
#include "librets/LoginResponse.h"
#include "librets/CapabilityUrls.h"

using namespace librets;
using namespace std;

#define CLASS LoginResponseTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testValidResponse);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testValidResponse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testValidResponse()
{
    istreamPtr inputStream = getResource("login_valid15.xml");
    LoginResponse response;
    response.Parse(inputStream);

    ASSERT_STRING_EQUAL("Joe Schmoe", response.GetValue("MemberName"));
    ASSERT_STRING_EQUAL("Joe Schmoe", response.GetValue("membername"));
    ASSERT_STRING_EQUAL("", response.GetValue("none"));
    ASSERT_STRING_EQUAL("Value", response.GetValue("X-Field"));
    
    ASSERT_STRING_EQUAL("Joe Schmoe", response.GetMemberName());
    ASSERT_STRING_EQUAL("Joe,NULL,NULL,NULL", response.GetUserInfo());
    ASSERT_STRING_EQUAL("", response.GetBroker());
    ASSERT_STRING_EQUAL("1.0.000", response.GetMetadataVersion());
    ASSERT_STRING_EQUAL("01 Jan 2000 01:02:00 GMT",
                        response.GetMetadataTimestamp());
    ASSERT_STRING_EQUAL("01 Jan 2000 01:01:00 GMT",
                        response.GetMinMetadataTimestamp());
    ASSERT_STRING_EQUAL("3,197.35", response.GetBalance());
    ASSERT_STRING_EQUAL("1800", response.GetTimeout());
    ASSERT_STRING_EQUAL("01 Jan 2000 01:03:00 GMT, 5",
                        response.GetPasswordExpire());
    ASSERT_STRING_EQUAL("chi", response.GetOfficeList());

    ASSERT_STRING_EQUAL("action",
                        response.GetActionUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/changePassword",
                        response.GetChangePasswordUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/getObject",
                        response.GetGetObjectUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/login",
                        response.GetLoginUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/loginComplete",
                        response.GetLoginCompleteUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/logout",
                        response.GetLogoutUrl());
    ASSERT_STRING_EQUAL("/rets/search",
                        response.GetSearchUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/getMetadata",
                        response.GetGetMetadataUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/serverInfo",
                        response.GetServerInformationUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/update",
                        response.GetUpdateUrl());

    CapabilityUrls expected("http://example.com:6103/");
    expected.SetActionUrl("http://example.com:6103/rets/action");
    expected.SetChangePasswordUrl(
        "http://example.com:6103/rets/changePassword");
    expected.SetGetObjectUrl("http://example.com:6103/rets/getObject");
    expected.SetLoginUrl("http://example.com:6103/rets/login");
    expected.SetLoginCompleteUrl("http://example.com:6103/rets/loginComplete");
    expected.SetLogoutUrl("http://example.com:6103/rets/logout");
    expected.SetSearchUrl("http://example.com:6103/rets/search");
    expected.SetGetMetadataUrl("http://example.com:6103/rets/getMetadata");
    expected.SetServerInformationUrl(
        "http://example.com:6103/rets/serverInfo");
    expected.SetUpdateUrl("http://example.com:6103/rets/update");

    CapabilityUrlsPtr actual =
        response.GetCapabilityUrls("http://example.com:6103/rets/login");
    CPPUNIT_ASSERT_EQUAL(expected, *actual);
}

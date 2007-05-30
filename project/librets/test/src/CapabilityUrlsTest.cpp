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
#include "librets/CapabilityUrls.h"

using namespace librets;
using namespace std;

#define CLASS CapabilityUrlsTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testResolvingUrls);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testNoSlashBase);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testResolvingUrls();
    void testEquals();
    void testNoSlashBase();

    string getBase();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

string CLASS::getBase()
{
    return "http://example.com:6103";
}

void CLASS::testResolvingUrls()
{
    CapabilityUrls urls(getBase() + "/rets/login");
    urls.SetActionUrl("");
    urls.SetChangePasswordUrl(getBase() + "/rets/changePassword");
    urls.SetGetObjectUrl("/rets/getObject");
    urls.SetSearchUrl("search");
    ASSERT_STRING_EQUAL("", urls.GetActionUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/changePassword",
                        urls.GetChangePasswordUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/getObject",
                        urls.GetGetObjectUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/search",
                        urls.GetSearchUrl());
}

void CLASS::testEquals()
{
    CapabilityUrls cu1(getBase() + "/rets/login");
    cu1.SetActionUrl(getBase() + "/rets/action");
    cu1.SetChangePasswordUrl(getBase() + "/rets/changePassword");
    cu1.SetGetObjectUrl(getBase() + "/rets/getObject");
    cu1.SetLoginUrl(getBase() + "/rets/login");
    cu1.SetLoginCompleteUrl(getBase() + "/rets/loginComplete");
    cu1.SetLogoutUrl(getBase() + "/rets/logout");
    cu1.SetSearchUrl(getBase() + "/rets/search");
    cu1.SetGetMetadataUrl(getBase() + "/rets/getMetadata");
    cu1.SetServerInformationUrl(getBase() + "/rets/serverInfo");
    cu1.SetUpdateUrl(getBase() + "/rets/update");

    CapabilityUrls cu2(getBase() + "/rets/login");
    cu2.SetActionUrl(getBase() + "/rets/action");
    cu2.SetChangePasswordUrl(getBase() + "/rets/changePassword");
    cu2.SetGetObjectUrl(getBase() + "/rets/getObject");
    cu2.SetLoginUrl(getBase() + "/rets/login");
    cu2.SetLoginCompleteUrl(getBase() + "/rets/loginComplete");
    cu2.SetLogoutUrl(getBase() + "/rets/logout");
    cu2.SetSearchUrl(getBase() + "/rets/search");
    cu2.SetGetMetadataUrl(getBase() + "/rets/getMetadata");
    cu2.SetServerInformationUrl(getBase() + "/rets/serverInfo");
    cu2.SetUpdateUrl(getBase() + "/rets/update");

    CapabilityUrls cu3(getBase() + "/rets/login");
    cu3.SetActionUrl("/rets/action");
    cu3.SetChangePasswordUrl("/rets/changePassword");
    cu3.SetGetObjectUrl("/rets/getObject");
    cu3.SetLoginUrl(getBase() + "/rets/login");
    cu3.SetLoginCompleteUrl("/rets/loginComplete");
    cu3.SetLogoutUrl("/rets/logout");
    cu3.SetSearchUrl("/rets/search");
    cu3.SetGetMetadataUrl("/rets/getMetadata");
    cu3.SetServerInformationUrl("/rets/serverInfo");
    cu3.SetUpdateUrl("/rets/update");

    CPPUNIT_ASSERT_EQUAL(cu1, cu2);
    CPPUNIT_ASSERT_EQUAL(cu1, cu3);
}

void CLASS::testNoSlashBase()
{
    CapabilityUrls urls(getBase());
    urls.SetActionUrl(getBase() + "/rets/action");
    urls.SetChangePasswordUrl(getBase() + "/rets/changePassword");
    urls.SetGetObjectUrl(getBase() + "/rets/getObject");
    urls.SetLoginUrl(getBase() + "/rets/login");
    urls.SetLoginCompleteUrl(getBase() + "/rets/loginComplete");
    urls.SetLogoutUrl(getBase() + "/rets/logout");
    urls.SetSearchUrl(getBase() + "/rets/search");
    urls.SetGetMetadataUrl(getBase() + "/rets/getMetadata");
    urls.SetServerInformationUrl(getBase() + "/rets/serverInfo");
    urls.SetUpdateUrl(getBase() + "/rets/update");

    ASSERT_STRING_EQUAL("http://example.com:6103/rets/action",
                        urls.GetActionUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/changePassword",
                        urls.GetChangePasswordUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/getObject",
                        urls.GetGetObjectUrl());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/search",
                        urls.GetSearchUrl());

    string portlessBase = "http://example.com";
    CapabilityUrls urls2(portlessBase);
    urls2.SetActionUrl(portlessBase + "/rets/action");
    urls2.SetChangePasswordUrl(portlessBase + "/rets/changePassword");
    urls2.SetGetObjectUrl(portlessBase + "/rets/getObject");
    urls2.SetLoginUrl(portlessBase + "/rets/login");
    urls2.SetLoginCompleteUrl(portlessBase + "/rets/loginComplete");
    urls2.SetLogoutUrl(portlessBase + "/rets/logout");
    urls2.SetSearchUrl(portlessBase + "/rets/search");
    urls2.SetGetMetadataUrl(portlessBase + "/rets/getMetadata");
    urls2.SetServerInformationUrl(portlessBase + "/rets/serverInfo");
    urls2.SetUpdateUrl(portlessBase + "/rets/update");

    ASSERT_STRING_EQUAL("http://example.com/rets/action",
                        urls2.GetActionUrl());
    ASSERT_STRING_EQUAL("http://example.com/rets/changePassword",
                        urls2.GetChangePasswordUrl());
    ASSERT_STRING_EQUAL("http://example.com/rets/getObject",
                        urls2.GetGetObjectUrl());
    ASSERT_STRING_EQUAL("http://example.com/rets/search",
                        urls2.GetSearchUrl());
}

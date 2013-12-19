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
#include "librets/RetsException.h"

using namespace librets;
using namespace std;

#define CLASS LoginResponseTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testInvalidLogin);
    CPPUNIT_TEST(testValid18Response);
    CPPUNIT_TEST(testValid15Response);
    CPPUNIT_TEST(testValid10Response);
    CPPUNIT_TEST(testShortResponse);
    CPPUNIT_TEST(testCrisResponse);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testInvalidLogin();
    void testValid18Response();
    void testValid15Response();
    void testValid10Response();
    void testShortResponse();
    void testCrisResponse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testInvalidLogin()
{
    istreamPtr inputStream = getResource("login-invalid.xml");
    LoginResponse response;
    response.Parse(inputStream, RETS_1_5);

    ASSERT_EQUAL(20036, response.GetRetsReplyCode());
    ASSERT_STRING_EQUAL("Miscellaneous Server Error", response.GetRetsReplyText());
}

void CLASS::testValid15Response()
{
    istreamPtr inputStream = getResource("login-valid15.xml");
    LoginResponse response;
    response.Parse(inputStream, RETS_1_5);
    
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
    
    CapabilityUrlsAPtr actual =
    response.CreateCapabilityUrls("http://example.com:6103/rets/login");
    CPPUNIT_ASSERT_EQUAL(expected, *actual);
}

void CLASS::testValid18Response()
{
    istreamPtr inputStream = getResource("login-valid18.xml");
    LoginResponse response;
    response.Parse(inputStream, RETS_1_8);
    
    ASSERT_STRING_EQUAL("Joe Schmoe", response.GetValue("MemberName"));
    ASSERT_STRING_EQUAL("Joe Schmoe", response.GetValue("membername"));
    ASSERT_STRING_EQUAL("", response.GetValue("none"));
    ASSERT_STRING_EQUAL("Value", response.GetValue("X-Field"));
    
    ASSERT_STRING_EQUAL("Joe Schmoe", response.GetMemberName());
    ASSERT_STRING_EQUAL("1.0.000", response.GetMetadataVersion());
    ASSERT_STRING_EQUAL("Sat, 01 Jan 2000 01:02:00 GMT",
                        response.GetMetadataTimestamp());
    ASSERT_STRING_EQUAL("Sat, 01 Jan 2000 01:01:00 GMT",
                        response.GetMinMetadataTimestamp());
    ASSERT_STRING_EQUAL("balance", response.GetBalance());
    ASSERT_STRING_EQUAL("123", response.GetTimeout());
    ASSERT_STRING_EQUAL("Sat, 01 Jan 2000 01:03:00 GMT",
                        response.GetPasswordExpire());
    ASSERT_STRING_EQUAL("Office List", response.GetOfficeList());
    ASSERT_STRING_EQUAL("MEMBERNAME;Character;Joe Schmoe\r\n"
                        "USERID;Character;1002284553\r\n"
                        "USERLEVEL;Int;25\r\n"
                        "USERCLASS;Character;RT\r\n"
                        "AGENTCODE;Character;RESOWG\r\n"
                        "BROKERCODE;Character;Test\r\n"
                        "BROKERBRANCH;Character;Test01\r\n"
                        "METADATAID;Character;76_31_31_26_TTFF_DMO\r\n"
                        "METADATAVERSION;Character;1.0.000\r\n"
                        "METADATATIMESTAMP;DateTime;Sat, 01 Jan 2000 01:02:00 GMT\r\n"
                        "MINMETADATATIMESTAMP;DateTime;Sat, 01 Jan 2000 01:01:00 GMT\r\n"
                        "USER;Character;RESOWG\r\n"
                        "Balance;Money;balance\r\n"
                        "TimeoutSeconds;Character;123\r\n"
                        "PasswordExpiration;DateTime;Sat, 01 Jan 2000 01:03:00 GMT\r\n"
                        "WarnPasswordExpirationDays;Character;42\r\n"
                        "OfficeList;Character;Office List\r\n"
                        "StandardNamesVersion;Character;FirstVersion\r\n"
                        "VendorName;Character;Some Vendor\r\n"
                        "ServerProductName;Character;My Product\r\n"
                        "ServerProductVersion;Character;1.2.3\r\n"
                        "OperatorName;Character;Joe Schmoe\r\n"
                        "RoleName;Character;Chief Cook\r\n"
                        "SupportContactInformation;Character;Do not call me\r\n",
                        response.GetSessionInformationTokens());
    
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
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/getPayloadList",
                        response.GetPayloadListUrl());
    
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
    expected.SetPayloadListUrl("http://example.com:6103/rets/getPayloadList");
    
    CapabilityUrlsAPtr actual =
    response.CreateCapabilityUrls("http://example.com:6103/rets/login");
    CPPUNIT_ASSERT_EQUAL(expected, *actual);
}

void CLASS::testValid10Response()
{
    istreamPtr inputStream = getResource("login-valid10.xml");
    LoginResponse response;
    response.Parse(inputStream, RETS_1_0);
    
    ASSERT_STRING_EQUAL("Elizabeth A Davis", response.GetValue("MemberName"));
    ASSERT_STRING_EQUAL("Elizabeth A Davis", response.GetValue("membername"));
    ASSERT_STRING_EQUAL("", response.GetValue("none"));
    
    ASSERT_STRING_EQUAL("Elizabeth A Davis", response.GetMemberName());
    ASSERT_STRING_EQUAL("272, AGENT:BROKER OFFICE:OFFICE, 50, 272",
                        response.GetUserInfo());
    ASSERT_STRING_EQUAL("MRIS,1", response.GetBroker());
    ASSERT_STRING_EQUAL("1.2.200", response.GetMetadataVersion());
    ASSERT_STRING_EQUAL("", response.GetMetadataTimestamp());
    ASSERT_STRING_EQUAL("", response.GetMinMetadataTimestamp());
    ASSERT_STRING_EQUAL("1800", response.GetTimeout());
    ASSERT_STRING_EQUAL("MRIS;1", response.GetOfficeList());
    
    ASSERT_STRING_EQUAL("", response.GetActionUrl());
    ASSERT_STRING_EQUAL(
        "http://cornerstone.mris.com:6103/platinum/changepassword",
        response.GetChangePasswordUrl());
    ASSERT_STRING_EQUAL("http://cornerstone.mris.com:6103/platinum/getobject",
                        response.GetGetObjectUrl());
    ASSERT_STRING_EQUAL("http://cornerstone.mris.com:6103/platinum/login",
                        response.GetLoginUrl());
    ASSERT_STRING_EQUAL("", response.GetLoginCompleteUrl());
    ASSERT_STRING_EQUAL("http://cornerstone.mris.com:6103/platinum/logout",
                        response.GetLogoutUrl());
    ASSERT_STRING_EQUAL("http://cornerstone.mris.com:6103/platinum/search",
                        response.GetSearchUrl());
    ASSERT_STRING_EQUAL("http://cornerstone.mris.com:6103/platinum/getmetadata",
                        response.GetGetMetadataUrl());
    ASSERT_STRING_EQUAL("", response.GetServerInformationUrl());
    ASSERT_STRING_EQUAL("", response.GetUpdateUrl());
    
    CapabilityUrls expected("http://cornerstone.mris.com:6103/platinum/");
    expected.SetActionUrl("");
    expected.SetChangePasswordUrl("changepassword");
    expected.SetGetObjectUrl("getobject");
    expected.SetLoginUrl("http://cornerstone.mris.com:6103/platinum/login");
    expected.SetLoginCompleteUrl("");
    expected.SetLogoutUrl("logout");
    expected.SetSearchUrl("search");
    expected.SetGetMetadataUrl("getmetadata");
    expected.SetServerInformationUrl("");
    expected.SetUpdateUrl("");
    
    CapabilityUrlsAPtr actual = response.CreateCapabilityUrls(
        "http://cornerstone.mris.com:6103/platinum/login");
    CPPUNIT_ASSERT_EQUAL(expected, *actual);
}

void CLASS::testShortResponse()
{
    istreamPtr inputStream = getResource("short-success-response.xml");
    LoginResponse response;
    try
    {
        response.Parse(inputStream, RETS_1_0);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsException &)
    {
        // Expected
    }
}

void CLASS::testCrisResponse()
{
    istreamPtr inputStream= getResource("login-cris.xml");
    LoginResponse response;
    response.Parse(inputStream, RETS_1_5);
}

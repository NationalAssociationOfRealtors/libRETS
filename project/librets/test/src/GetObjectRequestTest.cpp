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
#include "librets/GetObjectRequest.h"
#include "librets/RetsHttpRequest.h"

using namespace librets;
using namespace std;

#define CLASS GetObjectRequestTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testAddOneObject);
    CPPUNIT_TEST(testAddAllObjects);
    CPPUNIT_TEST(testAddMultipleObjects);
    CPPUNIT_TEST(testAddMultipleResourceEntities);
    CPPUNIT_TEST(testSetObjectData);
    CPPUNIT_TEST_SUITE_END();
    
  protected:
    void testAddOneObject();
    void testAddAllObjects();
    void testAddMultipleObjects();
    void testAddMultipleResourceEntities();
    void testSetObjectData();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testAddOneObject()
{
    GetObjectRequest request("Property", "Photo");
    request.AddObject("LN1", 1);
    CPPUNIT_ASSERT(request.HasDefaultObjectKeyAndId());
    ASSERT_STRING_EQUAL("LN1", request.GetDefaultObjectKey());
    CPPUNIT_ASSERT_EQUAL(1, request.GetDefaultObjectId());
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest(RETS_1_7);
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:1&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

void CLASS::testAddAllObjects()
{
    GetObjectRequest request("Property", "Photo");
    request.AddAllObjects("LN1");
    CPPUNIT_ASSERT(!request.HasDefaultObjectKeyAndId());
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest(RETS_1_7);
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:*&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

void CLASS::testAddMultipleObjects()
{
    GetObjectRequest request("Property", "Photo");
    request.AddObject("LN1", 1);
    request.AddObject("LN1", 2);
    CPPUNIT_ASSERT(!request.HasDefaultObjectKeyAndId());
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest(RETS_1_7);
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:1:2&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

void CLASS::testAddMultipleResourceEntities()
{
    GetObjectRequest request("Property", "Photo");
    request.AddObject("LN1", 1);
    request.AddAllObjects("LN2");
    CPPUNIT_ASSERT(!request.HasDefaultObjectKeyAndId());
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest(RETS_1_7);
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:1,LN2:*&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

void CLASS::testSetObjectData()
{
    GetObjectRequest request("Property", "Photo");
    request.AddObject("LN1", 1);
    request.AddAllObjects("LN2");
    request.SetObjectData("somedata,moredata");
    CPPUNIT_ASSERT(!request.HasDefaultObjectKeyAndId());
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest(RETS_1_8);
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:1,LN2:*&Location=0&ObjectData=somedata,moredata&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

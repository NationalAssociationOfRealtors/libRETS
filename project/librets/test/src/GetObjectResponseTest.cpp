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
#include "librets/GetObjectResponse.h"
#include "librets/ObjectDescriptor.h"
#include "librets/RetsException.h"
#include "librets/util.h"
#include "TestHttpResponse.h"

using namespace librets;
using namespace librets::util;
using namespace std;

#define CLASS GetObjectResponseTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testSinglePart);
    CPPUNIT_TEST(testSinglePartLocation);
    CPPUNIT_TEST(testSinglePartEmptyHeaders);
    CPPUNIT_TEST(testSinglePartEmptyHeadersNoDefaults);
    CPPUNIT_TEST(testSinglePartNoHeaders);
    CPPUNIT_TEST(testSinglePartNoHeadersNoDefaults);
    CPPUNIT_TEST(testUnsuccessfulResponse);
    CPPUNIT_TEST(testNoObjectFound);
    CPPUNIT_TEST(testNoObjectFoundFromVariman);
    CPPUNIT_TEST(testMultiPartLocation);
    CPPUNIT_TEST(testNormls);
    CPPUNIT_TEST(testCris);
    CPPUNIT_TEST_SUITE_END();
    
  protected:
    void testSinglePart();
    void testSinglePartLocation();
    void testSinglePartEmptyHeaders();
    void testSinglePartEmptyHeadersNoDefaults();
    void testSinglePartNoHeaders();
    void testSinglePartNoHeadersNoDefaults();
    void testUnsuccessfulResponse();
    void testNoObjectFound();
    void testNoObjectFoundFromVariman();
    void testMultiPartLocation();
    void testNormls();
    void testCris();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testSinglePart()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
         "get-object-response-single.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("abc123", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(1, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Front view", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    
    istreamPtr resource = getResource("abc123-1.gif");
    string expected = readIntoString(*resource);
    istream * data = objectDescriptor->GetDataStream();
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(*data);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testSinglePartLocation()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-single-location.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("abc123", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(1, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Front view", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("http://www.example.com/images/abc123.gif",
                        objectDescriptor->GetLocationUrl());
    
    istream * data = objectDescriptor->GetDataStream();
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(*data);
    ASSERT_STRING_EQUAL("", actual);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testSinglePartEmptyHeaders()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-single-empty-headers.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("foo", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(5, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Front view", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    
    istreamPtr resource = getResource("abc123-1.gif");
    string expected = readIntoString(*resource);
    istream * data = objectDescriptor->GetDataStream();
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(*data);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testSinglePartEmptyHeadersNoDefaults()
{
    try
    {
        GetObjectResponse response;
        RetsHttpResponsePtr httpResponse(new TestHttpResponse(
            "get-object-response-single-no-headers.txt"));
        response.Parse(httpResponse);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (const RetsException &)
    {
        // Expected
    }
}

void CLASS::testSinglePartNoHeaders()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-single-no-headers.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("foo", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(5, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Front view", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    
    istreamPtr resource = getResource("abc123-1.gif");
    string expected = readIntoString(*resource);
    istream * data = objectDescriptor->GetDataStream();
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(*data);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testSinglePartNoHeadersNoDefaults()
{
    try
    {
        GetObjectResponse response;
        RetsHttpResponsePtr httpResponse(new TestHttpResponse(
            "get-object-response-single-no-headers.txt"));
        response.Parse(httpResponse);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (const RetsException &)
    {
        // Expected
    }
}

void CLASS::testUnsuccessfulResponse()
{
    try
    {
        GetObjectResponse response;
        RetsHttpResponsePtr httpResponse(
            new TestHttpResponse("invalid-http-response.txt"));
        response.Parse(httpResponse);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (const RetsException &)
    {
        // Expetcted
    }
}

void CLASS::testNoObjectFound()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-no-object.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testNoObjectFoundFromVariman()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-no-object-variman.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testMultiPartLocation()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-multi-location.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    
    ObjectDescriptor * objectDescriptor;
    istream * dataStream;
    string data;
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("application/octet-stream",
                        objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("abc123", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(1, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Front view.", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("http://www.example.com/images/abc123/1",
                        objectDescriptor->GetLocationUrl());
    
    dataStream = objectDescriptor->GetDataStream();
    CPPUNIT_ASSERT(dataStream);
    data = readIntoString(*dataStream);
    ASSERT_STRING_EQUAL("", data);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("application/octet-stream",
                        objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("abc123", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(2, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Rear view.", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("http://www.example.com/images/abc123/2",
                        objectDescriptor->GetLocationUrl());
    
    dataStream = objectDescriptor->GetDataStream();
    CPPUNIT_ASSERT(dataStream);
    data = readIntoString(*dataStream);
    ASSERT_STRING_EQUAL("", data);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("application/octet-stream",
                        objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("abc123", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(3, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("http://www.example.com/images/abc123/3",
                        objectDescriptor->GetLocationUrl());
    
    dataStream = objectDescriptor->GetDataStream();
    CPPUNIT_ASSERT(dataStream);
    data = readIntoString(*dataStream);
    ASSERT_STRING_EQUAL("", data);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testNormls()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-normls.txt"));
    response.Parse(httpResponse);
    
    int objectCount = 0;
    ObjectDescriptor * objectDescriptor;
    while (objectDescriptor = response.NextObject())
    {
        objectCount++;
    }
    CPPUNIT_ASSERT_EQUAL(6, objectCount);
}

void CLASS::testCris()
{
    try 
    {
        GetObjectResponse response;
        RetsHttpResponsePtr httpResponse(new TestHttpResponse(
            "get-object-response-cris.txt"));
        response.Parse(httpResponse);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (const RetsException &)
    {
        // Expetcted
    }
}

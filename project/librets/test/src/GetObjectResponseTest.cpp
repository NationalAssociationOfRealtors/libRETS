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
    CPPUNIT_TEST(testMultiPartMalformed);
    CPPUNIT_TEST(testNormls);
    CPPUNIT_TEST(testCris);
    CPPUNIT_TEST(testRapattoni);
    CPPUNIT_TEST(testSinglePartRETS1_8);
    CPPUNIT_TEST(testMultiPartRETS1_8);
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
    void testMultiPartMalformed();
    void testNormls();
    void testCris();
    void testRapattoni();
    void testSinglePartRETS1_8();
    void testMultiPartRETS1_8();
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
    string expected = readIntoString(resource);
    istreamPtr data(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(data);
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
    
    istreamPtr data(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(data);
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
    string expected = readIntoString(resource);
    istreamPtr data(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(data);
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
    string expected = readIntoString(resource);
    istreamPtr data(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(data);
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
    RetsHttpResponsePtr httpResponse(
            new TestHttpResponse("invalid-http-response.txt"));

    try
    {
        GetObjectResponse response;
        response.Parse(httpResponse);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (const RetsException &)
    {
        // Expetcted
        ASSERT_EQUAL(500,httpResponse->GetResponseCode());
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
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_EQUAL(20403, objectDescriptor->GetRetsReplyCode());
}

void CLASS::testNoObjectFoundFromVariman()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-no-object-variman.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_EQUAL(20403, objectDescriptor->GetRetsReplyCode());
}

void CLASS::testMultiPartLocation()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-multi-location.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    
    ObjectDescriptor * objectDescriptor;
    string data;
    istreamPtr dataStream;

    
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
    data = readIntoString(dataStream);
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
    data = readIntoString(dataStream);
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
    data = readIntoString(dataStream);
    ASSERT_STRING_EQUAL("", data);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testMultiPartMalformed()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-multi-malformed.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse,true);
    
    ObjectDescriptor * objectDescriptor;
    string data;
    istreamPtr dataStream;

    
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
    data = readIntoString(dataStream);
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
    data = readIntoString(dataStream);
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
    data = readIntoString(dataStream);
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
    while ((objectDescriptor = response.NextObject()))
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
        CPPUNIT_FAIL("Malformed headers should cause an exception");
    }
    catch (const RetsException &)
    {
        // Expected
    }

    try
    {
        GetObjectResponse response;
        RetsHttpResponsePtr httpResponse(new TestHttpResponse(
            "get-object-response-cris.txt"));
        response.Parse(httpResponse, true);
    }
    catch (const RetsException &)
    {
        CPPUNIT_FAIL("Malformed headers should not cause an exception");
    }
        
}

void CLASS::testRapattoni()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
        "get-object-response-rapattoni.txt"));
    response.Parse(httpResponse);
    
    int objectCount = 0;
    ObjectDescriptor * objectDescriptor;
    while ((objectDescriptor = response.NextObject()))
    {
        objectCount++;
    }
    CPPUNIT_ASSERT_EQUAL(3, objectCount);
}

void CLASS::testSinglePartRETS1_8()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
                                                          "get-object-response-single-rets1-8.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("abc123", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(1, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Front view", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    ASSERT_STRING_EQUAL("Sub Description", objectDescriptor->GetSubDescription());
    ASSERT_EQUAL(1, (int)objectDescriptor->GetPreferred());
    ASSERT_STRING_EQUAL("caption for kitchen", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaCaption"));
    ASSERT_STRING_EQUAL("details about kitchen", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaDescription"));
    
    istreamPtr resource = getResource("abc123-1.gif");
    string expected = readIntoString(resource);
    istreamPtr data(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(data);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}

void CLASS::testMultiPartRETS1_8()
{
    GetObjectResponse response;
    RetsHttpResponsePtr httpResponse(new TestHttpResponse(
                                                          "get-object-response-multi-rets1-8.txt"));
    response.SetDefaultObjectKeyAndId("foo", 5);
    response.Parse(httpResponse);
    ObjectDescriptor * objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("2264256", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(0, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Front view", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    ASSERT_STRING_EQUAL("Sub Description", objectDescriptor->GetSubDescription());
    ASSERT_EQUAL(1, (int)objectDescriptor->GetPreferred());
    ASSERT_STRING_EQUAL("caption for kitchen", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaCaption"));
    ASSERT_STRING_EQUAL("details about kitchen", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaDescription"));
    ASSERT_STRING_EQUAL("UID", 
                        objectDescriptor->GetObjectData().GetUID());
    ASSERT_STRING_EQUAL("ObjectType", 
                        objectDescriptor->GetObjectData().GetObjectType());
    ASSERT_STRING_EQUAL("ResourceName", 
                        objectDescriptor->GetObjectData().GetResourceName());
    ASSERT_STRING_EQUAL("ResourceID", 
                        objectDescriptor->GetObjectData().GetResourceID());
    ASSERT_STRING_EQUAL("0", 
                        objectDescriptor->GetObjectData().GetObjectID());
    ASSERT_STRING_EQUAL("image/gif", 
                        objectDescriptor->GetObjectData().GetMimeType());
    ASSERT_EQUAL(1, static_cast<int>(
                        objectDescriptor->GetObjectData().GetIsDefault()));
    ASSERT_STRING_EQUAL("2013-11-29T01:02:03Z", 
                        objectDescriptor->GetObjectData().GetObjectModificationTimestamp());
    ASSERT_STRING_EQUAL("2013-11-29T01:03:04Z", 
                        objectDescriptor->GetObjectData().GetModificationTimestamp());
    ASSERT_STRING_EQUAL("0", 
                        objectDescriptor->GetObjectData().GetOrderHint());
    ASSERT_STRING_EQUAL("Description", 
                        objectDescriptor->GetObjectData().GetDescription());
    ASSERT_STRING_EQUAL("Caption", 
                        objectDescriptor->GetObjectData().GetCaption());
    ASSERT_STRING_EQUAL("1234", 
                        objectDescriptor->GetObjectData().GetFileSize());
    ASSERT_STRING_EQUAL("2345", 
                        objectDescriptor->GetObjectData().GetWidthPix());
    ASSERT_STRING_EQUAL("3456", 
                        objectDescriptor->GetObjectData().GetHeightPix());
    ASSERT_STRING_EQUAL("0", 
                        objectDescriptor->GetObjectData().GetDuration());
    ASSERT_STRING_EQUAL("8", 
                        objectDescriptor->GetObjectData().GetWidthInch());
    ASSERT_STRING_EQUAL("10", 
                        objectDescriptor->GetObjectData().GetHeightInch());
    ASSERT_STRING_EQUAL("custom header", 
                        objectDescriptor->GetObjectData().GetValue("X-Custom"));

    istreamPtr resource = getResource("abc123-1.gif");
    string expected = readIntoString(resource);
    istreamPtr data(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data);
    string actual = readIntoString(data);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    // Second image
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("2264256", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(1, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("Side view", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    ASSERT_STRING_EQUAL("Sub Description", objectDescriptor->GetSubDescription());
    ASSERT_EQUAL(0, (int)objectDescriptor->GetPreferred());
    ASSERT_STRING_EQUAL("caption for kitchen", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaCaption"));
    ASSERT_STRING_EQUAL("details about kitchen", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaDescription"));
    ASSERT_STRING_EQUAL("UID", 
                        objectDescriptor->GetObjectData().GetUID());
    ASSERT_STRING_EQUAL("ObjectType", 
                        objectDescriptor->GetObjectData().GetObjectType());
    ASSERT_STRING_EQUAL("ResourceName", 
                        objectDescriptor->GetObjectData().GetResourceName());
    ASSERT_STRING_EQUAL("ResourceID", 
                        objectDescriptor->GetObjectData().GetResourceID());
    ASSERT_STRING_EQUAL("1", 
                        objectDescriptor->GetObjectData().GetObjectID());
    ASSERT_STRING_EQUAL("image/gif", 
                        objectDescriptor->GetObjectData().GetMimeType());
    ASSERT_EQUAL(0, static_cast<int>(
                 objectDescriptor->GetObjectData().GetIsDefault()));
    ASSERT_STRING_EQUAL("2013-11-29T01:02:03Z", 
                        objectDescriptor->GetObjectData().GetObjectModificationTimestamp());
    ASSERT_STRING_EQUAL("2013-11-29T01:03:04Z", 
                        objectDescriptor->GetObjectData().GetModificationTimestamp());
    ASSERT_STRING_EQUAL("1", 
                        objectDescriptor->GetObjectData().GetOrderHint());
    ASSERT_STRING_EQUAL("Description", 
                        objectDescriptor->GetObjectData().GetDescription());
    ASSERT_STRING_EQUAL("Caption", 
                        objectDescriptor->GetObjectData().GetCaption());
    ASSERT_STRING_EQUAL("1234", 
                        objectDescriptor->GetObjectData().GetFileSize());
    ASSERT_STRING_EQUAL("2345", 
                        objectDescriptor->GetObjectData().GetWidthPix());
    ASSERT_STRING_EQUAL("3456", 
                        objectDescriptor->GetObjectData().GetHeightPix());
    ASSERT_STRING_EQUAL("0", 
                        objectDescriptor->GetObjectData().GetDuration());
    ASSERT_STRING_EQUAL("8", 
                        objectDescriptor->GetObjectData().GetWidthInch());
    ASSERT_STRING_EQUAL("10", 
                        objectDescriptor->GetObjectData().GetHeightInch());
    ASSERT_STRING_EQUAL("custom header", 
                        objectDescriptor->GetObjectData().GetValue("X-Custom"));
    
    istreamPtr data2(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data2);
    actual = readIntoString(data2);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    // Third image
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("2264256", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(2, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetSubDescription());
    ASSERT_EQUAL(0, (int)objectDescriptor->GetPreferred());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaCaption"));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaDescription"));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetUID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectType());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetResourceName());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetResourceID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetMimeType());
    ASSERT_EQUAL(0, static_cast<int>(objectDescriptor->GetObjectData().GetIsDefault()));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectModificationTimestamp());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetModificationTimestamp());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetOrderHint());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetDescription());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetCaption());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetFileSize());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetWidthPix());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetHeightPix());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetDuration());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetWidthInch());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetHeightInch());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("X-Custom"));
    
    istreamPtr data3(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data3);
    actual = readIntoString(data3);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    // Fourth Object doesn't exist. Verify the return codes.
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("text/xml", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("2264256", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(3, objectDescriptor->GetObjectId());
    
    ASSERT_EQUAL(20403, objectDescriptor->GetRetsReplyCode());
    ASSERT_STRING_EQUAL("No Object Found", objectDescriptor->GetRetsReplyText());
    
    // Fifth image
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("2264256", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(4, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetSubDescription());
    ASSERT_EQUAL(0, (int)objectDescriptor->GetPreferred());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaCaption"));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaDescription"));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetUID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectType());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetResourceName());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetResourceID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetMimeType());
    ASSERT_EQUAL(0, static_cast<int>(objectDescriptor->GetObjectData().GetIsDefault()));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectModificationTimestamp());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetModificationTimestamp());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetOrderHint());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetDescription());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetCaption());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetFileSize());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetWidthPix());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetHeightPix());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetDuration());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetWidthInch());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetHeightInch());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("X-Custom"));
    
    istreamPtr data4(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data4);
    actual = readIntoString(data4);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);

    // Last image
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(objectDescriptor);
    ASSERT_STRING_EQUAL("image/gif", objectDescriptor->GetContentType());
    ASSERT_STRING_EQUAL("2264256", objectDescriptor->GetObjectKey());
    ASSERT_EQUAL(5, objectDescriptor->GetObjectId());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetDescription());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetLocationUrl());
    ASSERT_STRING_EQUAL("", objectDescriptor->GetSubDescription());
    ASSERT_EQUAL(0, (int)objectDescriptor->GetPreferred());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaCaption"));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("PropMediaDescription"));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetUID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectType());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetResourceName());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetResourceID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectID());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetMimeType());
    ASSERT_EQUAL(0, static_cast<int>(objectDescriptor->GetObjectData().GetIsDefault()));
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetObjectModificationTimestamp());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetModificationTimestamp());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetOrderHint());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetDescription());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetCaption());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetFileSize());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetWidthPix());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetHeightPix());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetDuration());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetWidthInch());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetHeightInch());
    ASSERT_STRING_EQUAL("", 
                        objectDescriptor->GetObjectData().GetValue("X-Custom"));
    
    istreamPtr data5(objectDescriptor->GetDataStream());
    CPPUNIT_ASSERT(data5);
    actual = readIntoString(data5);
    ASSERT_EQUAL(size_t(94), actual.size());
    ASSERT_STRING_EQUAL(expected, actual);
    
    // Should be no more images.
    objectDescriptor = response.NextObject();
    CPPUNIT_ASSERT(!objectDescriptor);
}


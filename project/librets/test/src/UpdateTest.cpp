/*
 * Copyright (C) 2009 National Association of REALTORS(R)
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
#include "librets/UpdateRequest.h"
#include "librets/UpdateResponse.h"
#include "librets/RetsReplyException.h"
#include "librets/EncodingType.h"

using namespace librets;
using namespace std;

#define CLASS UpdateTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testErrorAndWarning);
    CPPUNIT_TEST(testErrorOnly);
    CPPUNIT_TEST(testRequest);
    CPPUNIT_TEST(testValidResponse);
    CPPUNIT_TEST(testWarningOnly);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testErrorAndWarning();
    void testErrorOnly();
    void testRequest();
    void testValidResponse();
    void testWarningOnly();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testErrorAndWarning()
{
    UpdateResponse result;
    istreamPtr inputStream = getResource("update-response-both.xml");
    result.Parse(inputStream);
    StringVector columns = result.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(4), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));
    ASSERT_STRING_EQUAL("ListPrice", columns.at(1));
    ASSERT_STRING_EQUAL("City", columns.at(2));
    ASSERT_STRING_EQUAL("ListDate", columns.at(3));
    
    CPPUNIT_ASSERT(result.HasNext());
    ASSERT_STRING_EQUAL("LN000005", result.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", result.GetString(0));
    ASSERT_STRING_EQUAL("250000", result.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("250000", result.GetString(1));
    ASSERT_STRING_EQUAL("Chicago", result.GetString("City"));
    ASSERT_STRING_EQUAL("Chicago", result.GetString(2));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString(3));
    
    CPPUNIT_ASSERT(!result.HasNext());
    
    CPPUNIT_ASSERT(result.HasNextError());
    ASSERT_STRING_EQUAL("ListPrice", result.GetErrorFieldName());
    ASSERT_EQUAL(10101, result.GetErrorNumber());
    ASSERT_EQUAL(0, result.GetErrorOffset());
    ASSERT_STRING_EQUAL("Are you kidding me?", result.GetErrorText());
    
    CPPUNIT_ASSERT(result.HasNextError());
    ASSERT_STRING_EQUAL("City", result.GetErrorFieldName());
    ASSERT_EQUAL(10102, result.GetErrorNumber());
    ASSERT_EQUAL(1, result.GetErrorOffset());
    ASSERT_STRING_EQUAL("Well, maybe.", result.GetErrorText());
    
    CPPUNIT_ASSERT(!result.HasNextError());
    
    CPPUNIT_ASSERT(result.HasNextWarning());
    ASSERT_STRING_EQUAL("ListPrice", result.GetWarningFieldName());
    ASSERT_EQUAL(202020, result.GetWarningNumber());
    ASSERT_EQUAL(0, result.GetWarningOffset());
    ASSERT_STRING_EQUAL("Raise it.", result.GetWarningText());
    ASSERT_EQUAL(2, result.GetWarningResponseRequired());
    
    CPPUNIT_ASSERT(result.HasNextWarning());
    ASSERT_STRING_EQUAL("ListDate", result.GetWarningFieldName());
    ASSERT_EQUAL(405060, result.GetWarningNumber());
    ASSERT_EQUAL(6, result.GetWarningOffset());
    ASSERT_STRING_EQUAL("Change date.", result.GetWarningText());
    ASSERT_EQUAL(0, result.GetWarningResponseRequired());
    
    CPPUNIT_ASSERT(!result.HasNextWarning());    
}

void CLASS::testErrorOnly()
{
    UpdateResponse result;
    istreamPtr inputStream = getResource("update-response-error.xml");
    result.Parse(inputStream);
    StringVector columns = result.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(4), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));
    ASSERT_STRING_EQUAL("ListPrice", columns.at(1));
    ASSERT_STRING_EQUAL("City", columns.at(2));
    ASSERT_STRING_EQUAL("ListDate", columns.at(3));
    
    CPPUNIT_ASSERT(result.HasNext());
    ASSERT_STRING_EQUAL("LN000005", result.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", result.GetString(0));
    ASSERT_STRING_EQUAL("250000", result.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("250000", result.GetString(1));
    ASSERT_STRING_EQUAL("Chicago", result.GetString("City"));
    ASSERT_STRING_EQUAL("Chicago", result.GetString(2));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString(3));
    
    CPPUNIT_ASSERT(!result.HasNext());
    
    CPPUNIT_ASSERT(result.HasNextError());
    ASSERT_STRING_EQUAL("ListPrice", result.GetErrorFieldName());
    ASSERT_EQUAL(10101, result.GetErrorNumber());
    ASSERT_EQUAL(0, result.GetErrorOffset());
    ASSERT_STRING_EQUAL("Are you kidding me?", result.GetErrorText());
    
    CPPUNIT_ASSERT(!result.HasNextError());
    
    CPPUNIT_ASSERT(!result.HasNextWarning());    
}

void CLASS::testRequest()
{
    UpdateRequest request("Property","ResidentialProperty");
    int flag = UpdateRequest::VALIDATE_ONLY;
    
    request.SetDelimiter("&");
    request.SetUpdateType("Add");
    request.SetValidateFlag(flag);
    
    request.SetField("Two","Value_Two");
    request.SetField("One","Value One");
    request.SetWarningResponse("2", "Response_Two");
    request.SetWarningResponse(1, "Response One");
    
    string record = request.ConstructRecord();
    string warnings = request.ConstructWarningResponse();
    
    ASSERT_STRING_EQUAL("&", request.GetDelimiter());
    ASSERT_STRING_EQUAL("Add", request.GetUpdateType());
    ASSERT_EQUAL(flag, request.GetValidateFlag());
    ASSERT_STRING_EQUAL("One=Value One&Two=Value_Two", record);
    ASSERT_STRING_EQUAL("1=Response One&2=Response_Two", warnings);
}

void CLASS::testValidResponse()
{
    UpdateResponse result;
    istreamPtr inputStream = getResource("update-response.xml");
    result.Parse(inputStream);
    StringVector columns = result.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(4), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));
    ASSERT_STRING_EQUAL("ListPrice", columns.at(1));
    ASSERT_STRING_EQUAL("City", columns.at(2));
    ASSERT_STRING_EQUAL("ListDate", columns.at(3));
    
    CPPUNIT_ASSERT(result.HasNext());
    ASSERT_STRING_EQUAL("LN000005", result.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", result.GetString(0));
    ASSERT_STRING_EQUAL("250000", result.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("250000", result.GetString(1));
    ASSERT_STRING_EQUAL("Chicago", result.GetString("City"));
    ASSERT_STRING_EQUAL("Chicago", result.GetString(2));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString(3));
    
    CPPUNIT_ASSERT(!result.HasNext());
    CPPUNIT_ASSERT(!result.HasNextError());
    CPPUNIT_ASSERT(!result.HasNextWarning());
}

void CLASS::testWarningOnly()
{
    UpdateResponse result;
    istreamPtr inputStream = getResource("update-response-warning.xml");
    result.Parse(inputStream);
    StringVector columns = result.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(4), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));
    ASSERT_STRING_EQUAL("ListPrice", columns.at(1));
    ASSERT_STRING_EQUAL("City", columns.at(2));
    ASSERT_STRING_EQUAL("ListDate", columns.at(3));
    
    CPPUNIT_ASSERT(result.HasNext());
    ASSERT_STRING_EQUAL("LN000005", result.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", result.GetString(0));
    ASSERT_STRING_EQUAL("250000", result.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("250000", result.GetString(1));
    ASSERT_STRING_EQUAL("Chicago", result.GetString("City"));
    ASSERT_STRING_EQUAL("Chicago", result.GetString(2));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-07-04", result.GetString(3));
    
    CPPUNIT_ASSERT(!result.HasNext());
    
    CPPUNIT_ASSERT(!result.HasNextError());
    
    CPPUNIT_ASSERT(result.HasNextWarning());
    ASSERT_STRING_EQUAL("ListPrice", result.GetWarningFieldName());
    ASSERT_EQUAL(202020, result.GetWarningNumber());
    ASSERT_EQUAL(0, result.GetWarningOffset());
    ASSERT_STRING_EQUAL("Raise it.", result.GetWarningText());
    ASSERT_EQUAL(2, result.GetWarningResponseRequired());
    
    CPPUNIT_ASSERT(!result.HasNextWarning());    
}


/*
 * Copyright (C) 2013 National Association of REALTORS(R)
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
#include "librets/PayloadListResultSet.h"
#include "librets/RetsReplyException.h"
#include "librets/EncodingType.h"

using namespace librets;
using namespace std;

#define CLASS PayloadListResultSetTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testValidResponse);
    CPPUNIT_TEST(testErrorResponse);
    CPPUNIT_TEST(testOutOfBoundsColumnNumber);
    CPPUNIT_TEST(testInvalidColumnName);
    CPPUNIT_TEST(testRetsStatus);
    CPPUNIT_TEST(testUTF8Response);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testValidResponse();
    void testErrorResponse();
    void testOutOfBoundsColumnNumber();
    void testInvalidColumnName();
    void testRetsStatus();
    void testUTF8Response();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testValidResponse()
{
    PayloadListResultSet resultSet;
    istreamPtr inputStream = getResource("payloadlist-response.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());
    
    ASSERT_STRING_EQUAL("foo", resultSet.GetPayloadClass());
    ASSERT_STRING_EQUAL("bar", resultSet.GetPayloadResource());
    ASSERT_STRING_EQUAL("01.02.33333", resultSet.GetPayloadVersion());
    ASSERT_STRING_EQUAL("2013-11-01T20:10:09Z", resultSet.GetPayloadDate());

    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(6), columns.size());
    ASSERT_STRING_EQUAL("PayloadName", columns.at(0));
    ASSERT_STRING_EQUAL("Resource", columns.at(1));
    ASSERT_STRING_EQUAL("Class", columns.at(2));
    ASSERT_STRING_EQUAL("Description", columns.at(3));
    ASSERT_STRING_EQUAL("URI", columns.at(4));
    ASSERT_STRING_EQUAL("MetadataEntryID", columns.at(5));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.0", resultSet.GetString("PayloadName"));
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.0", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString("Resource"));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("1", resultSet.GetString("Class"));
    ASSERT_STRING_EQUAL("1", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.0 Payload", resultSet.GetString("Description"));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.0 Payload", resultSet.GetString(3));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.0/Property", resultSet.GetString("URI"));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.0/Property", resultSet.GetString(4));
    ASSERT_STRING_EQUAL("Datadictionary10_Property_1", resultSet.GetString("MetadataEntryID"));
    ASSERT_STRING_EQUAL("Datadictionary10_Property_1", resultSet.GetString(5));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.1", resultSet.GetString("PayloadName"));
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.1", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString("Resource"));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("1", resultSet.GetString("Class"));
    ASSERT_STRING_EQUAL("1", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.1 Payload", resultSet.GetString("Description"));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.1 Payload", resultSet.GetString(3));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.1/Property", resultSet.GetString("URI"));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.1/Property", resultSet.GetString(4));
    ASSERT_STRING_EQUAL("Datadictionary11_Property_1", resultSet.GetString("MetadataEntryID"));
    ASSERT_STRING_EQUAL("Datadictionary11_Property_1", resultSet.GetString(5));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.0", resultSet.GetString("PayloadName"));
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.0", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString("Resource"));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("12", resultSet.GetString("Class"));
    ASSERT_STRING_EQUAL("12", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.0 Payload", resultSet.GetString("Description"));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.0 Payload", resultSet.GetString(3));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.0/Property", resultSet.GetString("URI"));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.0/Property", resultSet.GetString(4));
    ASSERT_STRING_EQUAL("Datadictionary10_Property_12", resultSet.GetString("MetadataEntryID"));
    ASSERT_STRING_EQUAL("Datadictionary10_Property_12", resultSet.GetString(5));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.1", resultSet.GetString("PayloadName"));
    ASSERT_STRING_EQUAL("DATADICTIONARY:1.1", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString("Resource"));
    ASSERT_STRING_EQUAL("Property", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("12", resultSet.GetString("Class"));
    ASSERT_STRING_EQUAL("12", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.1 Payload", resultSet.GetString("Description"));
    ASSERT_STRING_EQUAL("RESO Standard Data Dictionary 1.1 Payload", resultSet.GetString(3));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.1/Property", resultSet.GetString("URI"));
    ASSERT_STRING_EQUAL("http://www.reso.org/DataDictionary1.1/Property", resultSet.GetString(4));
    ASSERT_STRING_EQUAL("Datadictionary11_Property_12", resultSet.GetString("MetadataEntryID"));
    ASSERT_STRING_EQUAL("Datadictionary11_Property_12", resultSet.GetString(5));


    CPPUNIT_ASSERT(!resultSet.HasNext());
}

void CLASS::testErrorResponse()
{
    try
    {
        PayloadListResultSet resultSet;
        istreamPtr inputStream = getResource("payloadlist-response-error.xml");
        resultSet.Parse(inputStream);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsReplyException & e)
    {
        // Expected
        ASSERT_EQUAL(20500, e.GetReplyCode());
        ASSERT_STRING_EQUAL("Invalid Resource", e.GetMeaning());
    }
    /*
     * Test the extended error response.
     */
    try
    {
        PayloadListResultSet resultSet;
        resultSet.Parse(getResource("payloadlist-response-error-extended.xml"));
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsReplyException & e)
    {
        // Expected
        ASSERT_EQUAL(20500, e.GetReplyCode());
        ASSERT_STRING_EQUAL("No Metadata Found", e.GetMeaning());
        ASSERT_STRING_EQUAL("You can't do that, so try better next time.", e.GetExtendedMeaning());
    }
}

void CLASS::testOutOfBoundsColumnNumber()
{
    PayloadListResultSet resultSet;
    istreamPtr inputStream = getResource("payloadlist-response.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    CPPUNIT_ASSERT(resultSet.HasNext());
    try
    {
        resultSet.GetString(100);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (out_of_range &)
    {
        // Expected
    }
}

void CLASS::testInvalidColumnName()
{
    PayloadListResultSet resultSet;
    istreamPtr inputStream = getResource("payloadlist-response.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    CPPUNIT_ASSERT(resultSet.HasNext());
    try
    {
        resultSet.GetString("foo");
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (invalid_argument &)
    {
        // Expected
    }
}

void CLASS::testRetsStatus()
{
    /*
     * RETS-STATUS is used in PayloadList-repsonse.xml and data validation testing
     * has already happened. We can run this test against that data and
     * check for the presence of a reply code and reply text. We will also run 
     * the test againt PayloadList-response-no-count.xml, which also lacks the RETS-STATUS 
     * tag to make sure we properly reset the values.
     */
    PayloadListResultSet resultSet;
    istreamPtr inputStream = getResource("payloadlist-response.xml");
    resultSet.Parse(inputStream);
    /*
     * With the streaming enhancement, we can only be assured that the first status has
     * been seen. In this case, it should be normal (ok). Check to be sure.
     */
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
}

void CLASS::testUTF8Response()
{
    PayloadListResultSet resultSet;

    /*
     * This represents "nunez" with utf8 characters for the accented "u",
     * and the "enyay".
     */
    unsigned char nunez [] = {0x4e, 0xc3, 0xba, 0xc3, 0xb1, 0x65, 0x7a};
    std::string s_nunez(nunez, nunez + sizeof(nunez) / sizeof(nunez[0]));

    resultSet.SetEncoding(RETS_XML_DEFAULT_ENCODING);
    try
    {
        istreamPtr inputStream = getResource("search-response-extended-utf8.xml");
        resultSet.Parse(inputStream);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsException &)
    {
        // Expected
    }

    /*
     * Going to cheat and reuse the search response extended utf8 test. The point is 
     * that we don't care about the columns, we just care that we can parse UTF8 chars.
     */
    resultSet.SetEncoding(RETS_XML_UTF8_ENCODING);
    istreamPtr inputStream = getResource("search-response-extended-utf8.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(3), columns.size());
    ASSERT_STRING_EQUAL("AgentID", columns.at(0));
    ASSERT_STRING_EQUAL("FirstName", columns.at(1));
    ASSERT_STRING_EQUAL("LastName", columns.at(2));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("AG000001", resultSet.GetString("AgentID"));
    ASSERT_STRING_EQUAL("AG000001", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("Carlos", resultSet.GetString("FirstName"));
    ASSERT_STRING_EQUAL("Carlos", resultSet.GetString(1));
    ASSERT_STRING_EQUAL(s_nunez, resultSet.GetString("LastName"));
    ASSERT_STRING_EQUAL(s_nunez, resultSet.GetString(2));

    CPPUNIT_ASSERT(!resultSet.HasNext());
}


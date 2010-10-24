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
#include <stdexcept>
#include "testUtil.h"
#include "librets/SearchResultSet.h"
#include "librets/RetsReplyException.h"
#include "librets/EncodingType.h"

using namespace librets;
using namespace std;

#define CLASS SearchResultSetTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testValidResponse);
    CPPUNIT_TEST(testValidResponseNoCount);
    CPPUNIT_TEST(testNoRecordsFound);
    CPPUNIT_TEST(testErrorResponse);
    CPPUNIT_TEST(testSingleColumn);
    CPPUNIT_TEST(testOutOfBoundsColumnNumber);
    CPPUNIT_TEST(testInvalidColumnName);
    CPPUNIT_TEST(testPipeDelimiter);
    CPPUNIT_TEST(testExtendedCharResponse);
    CPPUNIT_TEST(testMaxRows);
    CPPUNIT_TEST(testRetsStatus);
    CPPUNIT_TEST(testUTF8Response);
    CPPUNIT_TEST(testInvalidCompactFormat);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testValidResponse();
    void testValidResponseNoCount();
    void testNoRecordsFound();
    void testErrorResponse();
    void testSingleColumn();
    void testOutOfBoundsColumnNumber();
    void testInvalidColumnName();
    void testPipeDelimiter();
    void testExtendedCharResponse();
    void testMaxRows();
    void testRetsStatus();
    void testUTF8Response();
    void testInvalidCompactFormat();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testValidResponse()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    ASSERT_EQUAL(2, resultSet.GetCount());
    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(4), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));
    ASSERT_STRING_EQUAL("ListPrice", columns.at(1));
    ASSERT_STRING_EQUAL("City", columns.at(2));
    ASSERT_STRING_EQUAL("ListDate", columns.at(3));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("250000", resultSet.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("250000", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("Chicago", resultSet.GetString("City"));
    ASSERT_STRING_EQUAL("Chicago", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("2004-07-04", resultSet.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-07-04", resultSet.GetString(3));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("380000", resultSet.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("380000", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("Buffalo Grove", resultSet.GetString("City"));
    ASSERT_STRING_EQUAL("Buffalo Grove", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("2004-05-30", resultSet.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-05-30", resultSet.GetString(3));

    CPPUNIT_ASSERT(!resultSet.HasNext());
}

void CLASS::testValidResponseNoCount()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response-no-count.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    ASSERT_EQUAL(-1, resultSet.GetCount());
    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(4), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));
    ASSERT_STRING_EQUAL("ListPrice", columns.at(1));
    ASSERT_STRING_EQUAL("City", columns.at(2));
    ASSERT_STRING_EQUAL("ListDate", columns.at(3));
    
    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("250000", resultSet.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("250000", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("Chicago", resultSet.GetString("City"));
    ASSERT_STRING_EQUAL("Chicago", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("2004-07-04", resultSet.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-07-04", resultSet.GetString(3));
    
    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString(0));
    ASSERT_STRING_EQUAL("380000", resultSet.GetString("ListPrice"));
    ASSERT_STRING_EQUAL("380000", resultSet.GetString(1));
    ASSERT_STRING_EQUAL("Buffalo Grove", resultSet.GetString("City"));
    ASSERT_STRING_EQUAL("Buffalo Grove", resultSet.GetString(2));
    ASSERT_STRING_EQUAL("2004-05-30", resultSet.GetString("ListDate"));
    ASSERT_STRING_EQUAL("2004-05-30", resultSet.GetString(3));
    
    CPPUNIT_ASSERT(!resultSet.HasNext());
}

void CLASS::testNoRecordsFound()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response-no-records.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetCount());
    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(0), columns.size());
    CPPUNIT_ASSERT(!resultSet.HasNext());
    ASSERT_EQUAL(20201, resultSet.GetReplyCode());
    ASSERT_STRING_EQUAL("No Records Found", resultSet.GetReplyText());
}

void CLASS::testErrorResponse()
{
    try
    {
        SearchResultSet resultSet;
        istreamPtr inputStream = getResource("search-response-error.xml");
        resultSet.Parse(inputStream);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsReplyException & e)
    {
        // Expected
        ASSERT_EQUAL(20205, e.GetReplyCode());
        ASSERT_STRING_EQUAL("Invalid Query Syntax", e.GetMeaning());
    }
    /*
     * Test the extended error response.
     */
    try
    {
        SearchResultSet resultSet;
        resultSet.Parse(getResource("search-response-error-extended.xml"));
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsReplyException & e)
    {
        // Expected
        ASSERT_EQUAL(20205, e.GetReplyCode());
        ASSERT_STRING_EQUAL("Invalid Query Syntax", e.GetMeaning());
        ASSERT_STRING_EQUAL("You can't do that, so try better next time.", e.GetExtendedMeaning());
    }
}

void CLASS::testSingleColumn()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response-single-column.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    ASSERT_EQUAL(3, resultSet.GetCount());
    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(1), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString(0));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString(0));
    
    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("", resultSet.GetString(0));
    
    CPPUNIT_ASSERT(!resultSet.HasNext());
}

void CLASS::testOutOfBoundsColumnNumber()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response.xml");
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
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response.xml");
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

void CLASS::testPipeDelimiter()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response-pipe.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(1), columns.size());
    ASSERT_STRING_EQUAL("CITY", columns.at(0));
    
    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("AURORA", resultSet.GetString("CITY"));
    ASSERT_STRING_EQUAL("AURORA", resultSet.GetString(0));
    
    CPPUNIT_ASSERT(!resultSet.HasNext());
}


void CLASS::testExtendedCharResponse()
{
    SearchResultSet resultSet;

    /*
     * This represents "nunez" with extended characters for the accented "u",
     * and the "enyay".
     */
    const char nunez [] = {0x4e, 0xc2, 0x9c, 0xc2, 0x96, 0x65, 0x7a, 0x00};

    resultSet.SetEncoding(RETS_XML_DEFAULT_ENCODING);
    try
    {
        istreamPtr inputStream = getResource("search-response-extended-char.xml");
        resultSet.Parse(inputStream);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsException & e)
    {
        // Expected
        CPPUNIT_ASSERT(!e.GetMessage().empty());
    }

    resultSet.SetEncoding(RETS_XML_ISO_ENCODING);
    istreamPtr inputStream = getResource("search-response-extended-char.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    ASSERT_EQUAL(1, resultSet.GetCount());
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
    ASSERT_STRING_EQUAL(nunez, resultSet.GetString("LastName"));
    ASSERT_STRING_EQUAL(nunez, resultSet.GetString(2));

    CPPUNIT_ASSERT(!resultSet.HasNext());
}

void CLASS::testMaxRows()
{
    /*
     * MAXROWS is set in search-repsonse.xml and data validation testing
     * has already happened. We can run this test against that data and
     * check for the presence of MAXROWS. We will also run the test againt
     * search-response-no-count.xml, which also lacks the MAXROWS tag make
     * sure we properly detect its lack of presense as well.
     */
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    // We should see the MAXROWS indication
    CPPUNIT_ASSERT(resultSet.HasMaxRows());

    /*
     * Now rerun the test against search-response-no-count.xml.
     * MAXROWS has not been set in this test.
     */
    inputStream = getResource("search-response-no-count.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    CPPUNIT_ASSERT(!resultSet.HasMaxRows());

}

void CLASS::testRetsStatus()
{
    /*
     * RETS-STATUS is used in search-repsonse.xml and data validation testing
     * has already happened. We can run this test against that data and
     * check for the presence of a reply code and reply text. We will also run 
     * the test againt search-response-no-count.xml, which also lacks the RETS-STATUS 
     * tag to make sure we properly reset the values.
     */
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response.xml");
    resultSet.Parse(inputStream);
    /*
     * With the streaming enhancement, we can only be assured that the first status has
     * been seen. In this case, it should be normal (ok). Check to be sure.
     */
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    
    /*
     * Now explicitly check for max rows. This will cause all of the data to be
     * processed.
     */

    CPPUNIT_ASSERT(resultSet.HasMaxRows());
    
    // We should see the replyCode == 20208, indicating MAXROWS have been sent
    ASSERT_EQUAL(20208, resultSet.GetReplyCode());
    // The ReplyText should be "Maximum Records Exceeded"
    ASSERT_STRING_EQUAL("Maximum Records Exceeded", resultSet.GetReplyText());

    /*
     * Now rerun the test against search-response-no-count.xml.
     * RETS-STATUS has not been set in this test.
     */
    inputStream = getResource("search-response-no-count.xml");
    resultSet.Parse(inputStream);
    CPPUNIT_ASSERT(resultSet.GetReplyCode() == 0);
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());
}

void CLASS::testUTF8Response()
{
    SearchResultSet resultSet;

    /*
     * This represents "nunez" with utf8 characters for the accented "u",
     * and the "enyay".
     */
    const char nunez [] = {0x4e, 0xc3, 0xba, 0xc3, 0xb1, 0x65, 0x7a, 0x00};

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

    resultSet.SetEncoding(RETS_XML_UTF8_ENCODING);
    istreamPtr inputStream = getResource("search-response-extended-utf8.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(0, resultSet.GetReplyCode());
    CPPUNIT_ASSERT(resultSet.GetReplyText().empty());

    ASSERT_EQUAL(1, resultSet.GetCount());
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
    ASSERT_STRING_EQUAL(nunez, resultSet.GetString("LastName"));
    ASSERT_STRING_EQUAL(nunez, resultSet.GetString(2));

    CPPUNIT_ASSERT(!resultSet.HasNext());
}

void CLASS::testInvalidCompactFormat()
{
    /*
     * Test a response where the tab is missing after the first tag and
     * before the data.
     */
    try
    {
        SearchResultSet resultSet;
        resultSet.SetEncoding (RETS_XML_DEFAULT_ENCODING);
        istreamPtr inputStream = getResource("search-response-no-leading-tab.xml");
        resultSet.Parse(inputStream);

        StringVector columns = resultSet.GetColumns();
        CPPUNIT_FAIL("Expected exception in testInvalidCompactFormat, missing initial tab.");
    }
    catch (RetsException &e)
    {
        // This text must match what is thrown by FixCompactArray.
        string errorText = "Invalid COMPACT format, missing initial tab";
        ASSERT_STRING_EQUAL(errorText, e.GetMessage().substr(0,errorText.length()));
    }
    
    /*
     * Create a response where the tab is missing beforer the last tag.
     */
    try
    {
        SearchResultSet resultSet;
        resultSet.SetEncoding (RETS_XML_DEFAULT_ENCODING);
        istreamPtr inputStream = getResource("search-response-no-final-tab.xml");
        resultSet.Parse(inputStream);

        StringVector columns = resultSet.GetColumns();
        CPPUNIT_FAIL("Expected exception in testInvalidCompactFormat, missing final tab.");
    }
    catch (RetsException &e)
    {
        // This text must match what is thrown by FixCompactArray.
        string errorText = "Invalid COMPACT format, missing final tab";
        ASSERT_STRING_EQUAL(errorText, e.GetMessage().substr(0,errorText.length()));
    }
    
}


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

using namespace librets;
using namespace std;

#define CLASS SearchResultSetTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testValidResponse);
    CPPUNIT_TEST(testNoRecordsFound);
    CPPUNIT_TEST(testErrorResponse);
    CPPUNIT_TEST(testSingleColumn);
    CPPUNIT_TEST(testOutOfBoundsColumnNumber);
    CPPUNIT_TEST(testInvalidColumnName);
    CPPUNIT_TEST(testPipeDelimiter);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testValidResponse();
    void testNoRecordsFound();
    void testErrorResponse();
    void testSingleColumn();
    void testOutOfBoundsColumnNumber();
    void testInvalidColumnName();
    void testPipeDelimiter();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testValidResponse()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response.xml");
    resultSet.Parse(inputStream);
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

void CLASS::testNoRecordsFound()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response-no-records.xml");
    resultSet.Parse(inputStream);
    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(0), columns.size());
    CPPUNIT_ASSERT(!resultSet.HasNext());
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

}

void CLASS::testSingleColumn()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response-single-column.xml");
    resultSet.Parse(inputStream);
    ASSERT_EQUAL(2, resultSet.GetCount());
    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(1), columns.size());
    ASSERT_STRING_EQUAL("ListingID", columns.at(0));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000005", resultSet.GetString(0));

    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString("ListingID"));
    ASSERT_STRING_EQUAL("LN000004", resultSet.GetString(0));

    CPPUNIT_ASSERT(!resultSet.HasNext());
}

void CLASS::testOutOfBoundsColumnNumber()
{
    SearchResultSet resultSet;
    istreamPtr inputStream = getResource("search-response.xml");
    resultSet.Parse(inputStream);
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
    StringVector columns = resultSet.GetColumns();
    ASSERT_EQUAL(StringVector::size_type(1), columns.size());
    ASSERT_STRING_EQUAL("CITY", columns.at(0));
    
    CPPUNIT_ASSERT(resultSet.HasNext());
    ASSERT_STRING_EQUAL("AURORA", resultSet.GetString("CITY"));
    ASSERT_STRING_EQUAL("AURORA", resultSet.GetString(0));
    
    CPPUNIT_ASSERT(!resultSet.HasNext());
}

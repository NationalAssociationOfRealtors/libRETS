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

#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "TestSqlMetadata.h"
#include "librets/SqlToDmqlCompiler.h"
#include "librets/DmqlQuery.h"
#include "librets/GetObjectQuery.h"
#include "librets/DmqlCriterion.h"
#include "librets/DmqlExpression.h"
#include "librets/RetsSqlException.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS SqlToDmqlCompilerTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testGreaterThan);
    CPPUNIT_TEST(testSelectColumns);
    CPPUNIT_TEST(testFullyQualifiedColumnNames);
    CPPUNIT_TEST(testInvalidColumnFQColumnNames);
    CPPUNIT_TEST(testOrderByIgnored);
    CPPUNIT_TEST(testLessThan);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testStringEquals);
    CPPUNIT_TEST(testLessThanOrEquals);
    CPPUNIT_TEST(testGreaterThanOrEquals);
    CPPUNIT_TEST(testLookupEquals);
    CPPUNIT_TEST(testInvalidTableNames);
    CPPUNIT_TEST(testComments);
    CPPUNIT_TEST(testOr);
    CPPUNIT_TEST(testAnd);
    CPPUNIT_TEST(testNot);
    CPPUNIT_TEST(testEmptyWhere);
    CPPUNIT_TEST(testQuotedLiterals);
    CPPUNIT_TEST(testTableAlias);
    CPPUNIT_TEST(testTableAliasWithoutAs);
    CPPUNIT_TEST(testInvalidTableAliases);
    CPPUNIT_TEST(testGetAllObjects);
    CPPUNIT_TEST(testGetOneObject);
    CPPUNIT_TEST(testGetTwoObjects);
    CPPUNIT_TEST_SUITE_END();

  public:
    CLASS();
    
  protected:
    void testGreaterThan();
    void testSelectColumns();
    void testFullyQualifiedColumnNames();
    void testInvalidColumnFQColumnNames();
    void testOrderByIgnored();
    void testLessThan();
    void testEquals();
    void testStringEquals();
    void testLessThanOrEquals();
    void testGreaterThanOrEquals();
    void testLookupEquals();
    void testInvalidTableNames();
    void testComments();
    void testOr();
    void testAnd();
    void testNot();
    void testEmptyWhere();
    void testQuotedLiterals();
    void testTableAlias();
    void testTableAliasWithoutAs();
    void testInvalidTableAliases();
    void testGetAllObjects();
    void testGetOneObject();
    void testGetTwoObjects();

    DmqlQueryPtr sqlToDmql(string sql);
    GetObjectQueryPtr sqlToGetObject(string sql);
    void assertInvalidSql(string sql,
                          const CPPUNIT_NS::SourceLine & sourceLine);

    SqlToDmqlCompilerPtr mCompiler;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

CLASS::CLASS()
{
    TestSqlMetadataPtr metadata(new TestSqlMetadata());
    mCompiler.reset(new SqlToDmqlCompiler(metadata));
}

DmqlQueryPtr CLASS::sqlToDmql(string sql)
{
    if (mCompiler->sqlToDmql(sql) != SqlToDmqlCompiler::DMQL_QUERY)
    {
        CPPUNIT_FAIL("Not a DMQL query");
    }
    return mCompiler->GetDmqlQuery();
}

GetObjectQueryPtr CLASS::sqlToGetObject(string sql)
{
    if (mCompiler->sqlToDmql(sql) != SqlToDmqlCompiler::GET_OBJECT_QUERY)
    {
        CPPUNIT_FAIL("Not a GetObject query");
    }
    return mCompiler->GetGetObjectQuery();
}

#define ASSERT_INVALID_SQL(_SQL_) assertInvalidSql(_SQL_, CPPUNIT_SOURCELINE())

void CLASS::assertInvalidSql(string sql,
                             const CPPUNIT_NS::SourceLine & sourceLine)
{
    try
    {
        istringstream inputStream(sql);
        mCompiler->sqlToDmql(inputStream);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message("forced failure",
                                "should have thrown exception"),
            sourceLine);
    }
    catch (RetsSqlException &)
    {
        // Expected
    }
}

void CLASS::testGreaterThanOrEquals()
{
    DmqlQueryPtr query =
    sqlToDmql("select * "
              " from data:Property:RES "
              " where ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testGreaterThan()
{
    DmqlQueryPtr query =
        sqlToDmql("select * from data:Property:RES where ListPrice > 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion =
        logicAnd(gt("ListPrice", literal(300000)),
                 logicNot(eq("ListPrice", literal(300000))));;
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testLookupEquals()
{
    DmqlQueryPtr query =
    sqlToDmql("select * "
              " from data:Property:RES "
              " where Status = 'Active';");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = lookupOr("Status", literal("Active"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testSelectColumns()
{
    DmqlQueryPtr query =
        sqlToDmql("select ListingID, ListPrice "
                  "  from data:Property:RES "
                  " where ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    columns.push_back("ListingID");
    columns.push_back("ListPrice");
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testFullyQualifiedColumnNames()
{
    DmqlQueryPtr query =
    sqlToDmql("select data:Property:RES.ListingID, data:Property:RES.ListPrice "
              "  from data:Property:RES "
              " where data:Property:RES.ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    columns.push_back("ListingID");
    columns.push_back("ListPrice");
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testInvalidColumnFQColumnNames()
{
    ASSERT_INVALID_SQL(
        "select invalid.ListingID, data:Property:RES.ListPrice "
        "  from data:Property:RES "
        " where data:Property:RES.ListPrice >= 300000;");
    ASSERT_INVALID_SQL(
        "select data:Property:RES.ListingID, data:Property:RES.ListPrice "
        "  from data:Property:RES "
        " where invalid.ListPrice >= 300000;");
}

void CLASS::testOrderByIgnored()
{
    DmqlQueryPtr query =
    sqlToDmql("select * "
              " from data:Property:RES "
              " where ListPrice >= 300000 "
              "order by ListPrice, data:Property:RES.ListingID;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}


void CLASS::testLessThanOrEquals()
{
    DmqlQueryPtr query =
    sqlToDmql("select * "
              "  from data:Property:RES "
              " where ListPrice <= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = lt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testLessThan()
{
    DmqlQueryPtr query =
        sqlToDmql("select * "
                  "  from data:Property:RES "
                  " where ListPrice < 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion =
        logicAnd(lt("ListPrice", literal("300000")),
                 logicNot(eq("ListPrice", literal("300000"))));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testEquals()
{
    DmqlQueryPtr query =
        sqlToDmql("select * "
                  "  from data:Property:RES "
                  " where ListPrice = 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion = eq("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testStringEquals()
{
    DmqlQueryPtr query =
        sqlToDmql("select * "
                  "  from data:Property:RES "
                  " where ListingID = 'LN000002';");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    DmqlCriterionPtr criterion = eq("ListingID", literal("LN000002"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testInvalidTableNames()
{
    // Table names must have 3 sections, separated by ":"
    ASSERT_INVALID_SQL("select * from rets_Property where a > 0");
    ASSERT_INVALID_SQL("select * from data:Property:RES:foo where a > 0");
    // Table names must begin with "data:"
    ASSERT_INVALID_SQL("select * from foo:Property:RES where a > 0");
}

void CLASS::testComments()
{
    DmqlQueryPtr query =
        sqlToDmql("select * -- Put fields to select here\n"
                  "from data:Property:RES \n"
                  "\n -- ignore this! \n"
                  "where ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testOr()
{
    DmqlQueryPtr query =
        sqlToDmql("select * "
                  "  from data:Property:RES "
                  " where ListingID = 'LN1' OR ListingID = 'LN2';");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion = logicOr(eq("ListingID", literal("LN1")),
                                         eq("ListingID", literal("LN2")));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testAnd()
{
    DmqlQueryPtr query =
        sqlToDmql("select * "
                  "  from data:Property:RES "
                  " where ListPrice >= 300000 AND ListPrice <= 400000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion = logicAnd(gt("ListPrice", literal(300000)),
                                          lt("ListPrice", literal(400000)));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testNot()
{
    DmqlQueryPtr query =
        sqlToDmql("select * "
                  "  from data:Property:RES "
                  " where not ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());

    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion = logicNot(gt("ListPrice", literal(300000)));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testEmptyWhere()
{
    DmqlQueryPtr query =
    sqlToDmql("select * "
              " from data:Property:RES;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());

    DmqlCriterionPtr criterion = query->GetCriterion();
    CPPUNIT_ASSERT(!criterion);
}

void CLASS::testQuotedLiterals()
{
    DmqlQueryPtr query =
        sqlToDmql("select ListingID, \"data:Property:RES\".ListPrice "
                  " from \"data:Property:RES\" "
                  " where \"data:Property:RES\".ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    columns.push_back("ListingID");
    columns.push_back("ListPrice");
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testTableAlias()
{
    DmqlQueryPtr query =
        sqlToDmql("select res.ListingID, res.ListPrice "
                  " from \"data:Property:RES\" as res"
                  " where res.ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    columns.push_back("ListingID");
    columns.push_back("ListPrice");
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testTableAliasWithoutAs()
{
    DmqlQueryPtr query =
        sqlToDmql("select res.ListingID, res.ListPrice "
                  " from \"data:Property:RES\" res"
                  " where res.ListPrice >= 300000;");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("RES", query->GetClass());
    
    StringVector columns;
    columns.push_back("ListingID");
    columns.push_back("ListPrice");
    ASSERT_VECTOR_EQUAL(columns, *query->GetFields());
    
    DmqlCriterionPtr criterion = gt("ListPrice", literal("300000"));
    ASSERT_EQUAL(*criterion, *query->GetCriterion());
}

void CLASS::testInvalidTableAliases()
{
    // If you use an alias, you can no longer use the original table name
    ASSERT_INVALID_SQL("select * "
                       "from data:Property:RES r "
                       "where data:Property:RES.ListPrice > 0");
    
    ASSERT_INVALID_SQL("select  "
                       "from data:Property:RES r "
                       "where res.ListPrice > 0");
    
    ASSERT_INVALID_SQL("select res.ListPrice "
                       "from data:Property:RES r "
                       "where r.ListPrice > 0");
}

void CLASS::testGetAllObjects()
{
    GetObjectQueryPtr query = 
        sqlToGetObject("select * "
                       "  from object:Property "
                       " where type = 'Photo' "
                       "       and object_key = 'LN1'");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_STRING_EQUAL("LN1", query->GetObjectKey());
    IntVector objectIds;
    ASSERT_VECTOR_EQUAL(objectIds, *query->GetObjectIds());
}

void CLASS::testGetOneObject()
{
    GetObjectQueryPtr query = 
    sqlToGetObject("select * "
                   "  from object:Property "
                   " where type = 'Photo' and object_key = 'LN1'"
                   "       and object_id = 1");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_STRING_EQUAL("LN1", query->GetObjectKey());
    IntVector objectIds;
    objectIds.push_back(1);
    ASSERT_VECTOR_EQUAL(objectIds, *query->GetObjectIds());
}

void CLASS::testGetTwoObjects()
{
    GetObjectQueryPtr query = 
    sqlToGetObject("select * "
                   "  from object:Property "
                   " where type = 'Photo' and object_key = 'LN1'"
                   "       and object_id = 1 or object_id = 2");
    ASSERT_STRING_EQUAL("Property", query->GetResource());
    ASSERT_STRING_EQUAL("Photo", query->GetType());
    ASSERT_STRING_EQUAL("LN1", query->GetObjectKey());
    IntVector objectIds;
    objectIds.push_back(1);
    objectIds.push_back(2);
    ASSERT_VECTOR_EQUAL(objectIds, *query->GetObjectIds());
}

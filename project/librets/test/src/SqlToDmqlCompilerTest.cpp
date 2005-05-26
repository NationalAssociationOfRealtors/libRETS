#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
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
    CPPUNIT_TEST(testLessThan);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testStringEquals);
    CPPUNIT_TEST(testLessThanOrEquals);
    CPPUNIT_TEST(testGreaterThanOrEquals);
    CPPUNIT_TEST(testInvalidTableNames);
    CPPUNIT_TEST(testComments);
    CPPUNIT_TEST(testOr);
    CPPUNIT_TEST(testAnd);
    CPPUNIT_TEST(testNot);
    CPPUNIT_TEST(testGetAllObjects);
    CPPUNIT_TEST(testGetOneObject);
    CPPUNIT_TEST(testGetTwoObjects);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testGreaterThan();
    void testSelectColumns();
    void testLessThan();
    void testEquals();
    void testStringEquals();
    void testLessThanOrEquals();
    void testGreaterThanOrEquals();
    void testInvalidTableNames();
    void testComments();
    void testOr();
    void testAnd();
    void testNot();
    void testGetAllObjects();
    void testGetOneObject();
    void testGetTwoObjects();

    DmqlQueryPtr sqlToDmql(string sql);
    GetObjectQueryPtr sqlToGetObject(string sql);
    void assertInvalidSql(string sql,
                          const CPPUNIT_NS::SourceLine & sourceLine);
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

DmqlQueryPtr CLASS::sqlToDmql(string sql)
{
    SqlToDmqlCompiler compiler;
    if (compiler.sqlToDmql(sql) != SqlToDmqlCompiler::DMQL_QUERY)
    {
        CPPUNIT_FAIL("Not a DMQL query");
    }
    return compiler.GetDmqlQuery();
}

GetObjectQueryPtr CLASS::sqlToGetObject(string sql)
{
    SqlToDmqlCompiler compiler;
    if (compiler.sqlToDmql(sql) != SqlToDmqlCompiler::GET_OBJECT_QUERY)
    {
        CPPUNIT_FAIL("Not a GetObject query");
    }
    return compiler.GetGetObjectQuery();
}

#define ASSERT_INVALID_SQL(_SQL_) assertInvalidSql(_SQL_, CPPUNIT_SOURCELINE())

void CLASS::assertInvalidSql(string sql,
                             const CPPUNIT_NS::SourceLine & sourceLine)
{
    try
    {
        SqlToDmqlCompiler compiler;
        istringstream inputStream(sql);
        compiler.sqlToDmql(inputStream);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message("forced failure",
                                "should have throw exception"),
            sourceLine);
    }
    catch (RetsSqlException & e)
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

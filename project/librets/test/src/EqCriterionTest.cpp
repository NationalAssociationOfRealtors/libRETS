#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "librets/sql_forward.h"
#include "librets/EqCriterion.h"
#include "librets/LiteralCriterion.h"
#include "librets/DmqlExpression.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS EqCriterionTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testToDmql);
    CPPUNIT_TEST(testDmqlExpression);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testEquals();
    void testToDmql();
    void testDmqlExpression();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testEquals()
{
    LiteralCriterionPtr foo(new LiteralCriterion("foo"));
    LiteralCriterionPtr bar(new LiteralCriterion("bar"));
    EqCriterion eq1("field1", foo);
    EqCriterion eq2("field1", foo);
    EqCriterion eq3("field1", bar);
    EqCriterion eq4("field2", foo);
    EqCriterion eq5("field2", bar);

    ASSERT_EQUAL(eq1, eq2);
    ASSERT_NOT_EQUAL(eq1, eq3);
    ASSERT_NOT_EQUAL(eq1, eq4);
    ASSERT_NOT_EQUAL(eq1, eq5);
    ASSERT_NOT_EQUAL(eq3, eq4);
    ASSERT_NOT_EQUAL(eq3, eq5);
}

void CLASS::testToDmql()
{
    LiteralCriterionPtr one(new LiteralCriterion("1"));
    EqCriterion eq("field", one);
    ASSERT_STRING_EQUAL("(field=1)", eq.ToDmqlString());
}

void CLASS::testDmqlExpression()
{
    DmqlCriterionPtr c1 = eq("field1", literal("1"));
    DmqlCriterionPtr c2 = eq("field1", literal(1));
    DmqlCriterionPtr c3 = eq("field1", literal("2"));

    ASSERT_EQUAL(*c1, *c2);
    ASSERT_NOT_EQUAL(*c1, *c3);
}

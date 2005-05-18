#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "librets/sql_forward.h"
#include "librets/NotCriterion.h"
#include "librets/LiteralCriterion.h"
#include "librets/DmqlExpression.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS NotCriterionTest

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
    NotCriterion not1(literal("foo"));
    NotCriterion not2(literal("foo"));
    NotCriterion not3(literal("bar"));

    ASSERT_EQUAL(not1, not2);
    ASSERT_NOT_EQUAL(not1, not3);
}

void CLASS::testToDmql()
{
    NotCriterion criterion(literal("foo"));
    ASSERT_STRING_EQUAL("~foo", criterion.ToDmqlString());
}

void CLASS::testDmqlExpression()
{
    DmqlCriterionPtr c1 = logicNot(literal("1"));
    DmqlCriterionPtr c2 = logicNot(literal(1));
    DmqlCriterionPtr c3 = logicNot(literal("2"));

    ASSERT_EQUAL(*c1, *c2);
    ASSERT_NOT_EQUAL(*c1, *c3);
}

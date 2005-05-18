#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "librets/sql_forward.h"
#include "librets/LiteralCriterion.h"

using namespace librets;
using namespace std;

#define CLASS LiteralCriterionTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testToDmql);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testEquals();
    void testToDmql();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testEquals()
{
    LiteralCriterion literal1;
    LiteralCriterion literal2;
    LiteralCriterion literal3("foo");
    LiteralCriterion literal4("foo");

    ASSERT_EQUAL(literal1, literal2);
    ASSERT_EQUAL(literal3, literal4);
    ASSERT_NOT_EQUAL(literal1, literal3);
    ASSERT_NOT_EQUAL(literal2, literal4);
}

void CLASS::testToDmql()
{
    LiteralCriterion literal1;
    LiteralCriterion literal2("foo");
    ASSERT_STRING_EQUAL("", literal1.ToDmqlString());
    ASSERT_STRING_EQUAL("foo", literal2.ToDmqlString());
}

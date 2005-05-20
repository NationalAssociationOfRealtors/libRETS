#include <sstream>
#include <cppunit/extensions/HelperMacros.h>
#include "librets/RetsObject.h"
#include "testUtil.h"
#include "TestObject.h"

using namespace librets;
using namespace std;

#define CLASS RetsObjectTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testPrint);
    CPPUNIT_TEST(testPrintOperator);
    CPPUNIT_TEST(testPrintOperatorPtr);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testEqualsOperator);
    CPPUNIT_TEST(testEqualsOperatorPtr);
    CPPUNIT_TEST(testDerivedPrintOperator);
    CPPUNIT_TEST(testDerivedPrintOperatorPtr);
    CPPUNIT_TEST(testDerivedEqualsOperator);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testPrint();
    void testPrintOperator();
    void testPrintOperatorPtr();
    void testEquals();
    void testEqualsOperator();
    void testEqualsOperatorPtr();
    void testDerivedPrintOperator();
    void testDerivedPrintOperatorPtr();
    void testDerivedEqualsOperator();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testPrint()
{
    RetsObject object;

    ostringstream expected;
    expected << "RetsObject[" << (void *) &object << "]";

    ostringstream actual;
    object.Print(actual);
    ASSERT_STRING_EQUAL(expected.str(), actual.str());
}

void CLASS::testPrintOperator()
{
    RetsObject object;

    ostringstream expected;
    expected << "RetsObject[" << (void *) &object << "]";

    ostringstream actual;
    actual << object;
    ASSERT_STRING_EQUAL(expected.str(), actual.str());
}

void CLASS::testPrintOperatorPtr()
{
    RetsObjectPtr object(new RetsObject());

    ostringstream expected;
    expected << "RetsObject[" << (void *) object.get() << "]";

    ostringstream actual;
    actual << object;
    ASSERT_STRING_EQUAL(expected.str(), actual.str());

    object.reset();
    expected.str("null");
    actual.str("");
    actual << object;
    ASSERT_STRING_EQUAL(expected.str(), actual.str());
}

void CLASS::testEquals()
{
    RetsObject object1;
    RetsObject object2;
    CPPUNIT_ASSERT(object1.Equals(&object1));
    CPPUNIT_ASSERT(object2.Equals(&object2));
    CPPUNIT_ASSERT(!(object1.Equals(&object2)));
    CPPUNIT_ASSERT(!(object2.Equals(&object1)));
    CPPUNIT_ASSERT(!(object1.Equals(0)));
    CPPUNIT_ASSERT(!(object2.Equals(0)));
}

void CLASS::testEqualsOperator()
{
    RetsObject object1;
    RetsObject object2;
    ASSERT_EQUAL(object1, object1);
    ASSERT_EQUAL(object2, object2);
    ASSERT_NOT_EQUAL(object1, object2);
    ASSERT_NOT_EQUAL(object2, object1);
}

void CLASS::testEqualsOperatorPtr()
{
    RetsObjectPtr object1(new RetsObject());
    RetsObjectPtr object2(new RetsObject());
    RetsObjectPtr null;
    CPPUNIT_ASSERT_EQUAL(object1, object1);
    CPPUNIT_ASSERT_EQUAL(object2, object2);
    bool equal = (object1 == object2);
    CPPUNIT_ASSERT(!equal);
    equal = (object2 == object1);
    CPPUNIT_ASSERT(!equal);
    equal = (object1 == null);
    CPPUNIT_ASSERT(!equal);
    equal = (object2 == null);
    CPPUNIT_ASSERT(!equal);
    equal = (null == object1);
    CPPUNIT_ASSERT(!equal);
}

void CLASS::testDerivedPrintOperator()
{
    TestObject object(1);

    ostringstream expected;
    expected << "TestObject: 1";

    ostringstream actual;
    actual << object;
    ASSERT_STRING_EQUAL(expected.str(), actual.str());
}

void CLASS::testDerivedPrintOperatorPtr()
{
    TestObjectPtr object(new TestObject(1));

    ostringstream expected;
    expected << "TestObject: 1";

    ostringstream actual;
    actual << object;
    ASSERT_STRING_EQUAL(expected.str(), actual.str());
}

void CLASS::testDerivedEqualsOperator()
{
    TestObject object1(1);
    TestObject object2(2);
    TestObject object3(1);
    ASSERT_EQUAL(object1, object1);
    ASSERT_EQUAL(object2, object2);
    ASSERT_EQUAL(object3, object3);
    ASSERT_EQUAL(object1, object3);
    ASSERT_EQUAL(object3, object1);
    ASSERT_NOT_EQUAL(object1, object2);
    ASSERT_NOT_EQUAL(object2, object1);
}

#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include "librets/xml_forward.h"
#include "librets/util.h"
#include "testUtil.h"
#include "TestObject.h"

using namespace librets;
using namespace librets::util;
using namespace std;

typedef vector<RetsXmlEventPtr> EventVector;

#define CLASS UtilTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testVectorEquals);
    CPPUNIT_TEST(testSplitField);
    CPPUNIT_TEST(testUrlEncode);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testVectorEquals();
    void testSplitField();
    void testUrlEncode();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testVectorEquals()
{
    TestObjectPtr derived;
    vector<TestObjectPtr> v1;
    derived.reset(new TestObject(1));
    v1.push_back(derived);
    derived.reset(new TestObject(2));
    v1.push_back(derived);
    derived.reset(new TestObject(3));
    v1.push_back(derived);

    vector<TestObjectPtr> v2;
    derived.reset(new TestObject(1));
    v2.push_back(derived);
    derived.reset(new TestObject(2));
    v2.push_back(derived);
    derived.reset(new TestObject(3));
    v2.push_back(derived);

    vector<TestObjectPtr> v3;
    derived.reset(new TestObject(3));
    v3.push_back(derived);
    derived.reset(new TestObject(2));
    v3.push_back(derived);
    derived.reset(new TestObject(1));
    v3.push_back(derived);
    
    vector<TestObjectPtr> v4;
    derived.reset(new TestObject(1));
    v4.push_back(derived);
    derived.reset(new TestObject(2));
    v4.push_back(derived);
    derived.reset(new TestObject(3));
    v4.push_back(derived);
    derived.reset(new TestObject(4));
    v4.push_back(derived);
    
    vector<TestObjectPtr> v5;
    derived.reset(new TestObject(1));
    v5.push_back(derived);
    derived.reset(new TestObject(2));
    v5.push_back(derived);

    CPPUNIT_ASSERT(VectorEquals(v1, v2));
    CPPUNIT_ASSERT(!VectorEquals(v1, v3));
    CPPUNIT_ASSERT(!VectorEquals(v1, v4));
    CPPUNIT_ASSERT(!VectorEquals(v1, v4));
}

void CLASS::testSplitField()
{
    string key;
    string value;
    CPPUNIT_ASSERT(splitField("foo = bar", "=", key, value));
    ASSERT_STRING_EQUAL("foo ", key);
    ASSERT_STRING_EQUAL(" bar", value);
    CPPUNIT_ASSERT(!splitField("foo = bar", ":", key, value));
}

void CLASS::testUrlEncode()
{
    ASSERT_STRING_EQUAL("foo%2bbar", urlEncode("foo+bar"));
}

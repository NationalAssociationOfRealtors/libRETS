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
    CPPUNIT_TEST(testTrim);
    CPPUNIT_TEST(testSplit);
    CPPUNIT_TEST(testSplitWithLimit);
    CPPUNIT_TEST(testSplitNoEmpties);
    CPPUNIT_TEST(testToUpper);
    CPPUNIT_TEST(testStartsWith);
    CPPUNIT_TEST(testUrlEncode);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testVectorEquals();
    void testTrim();
    void testSplit();
    void testSplitWithLimit();
    void testSplitNoEmpties();
    void testToUpper();
    void testStartsWith();
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

void CLASS::testTrim()
{
    ASSERT_STRING_EQUAL("", trim(""));
    ASSERT_STRING_EQUAL("", trim("  "));
    ASSERT_STRING_EQUAL("", trim("  \n\r\t"));
    ASSERT_STRING_EQUAL("foo", trim("foo"));
    ASSERT_STRING_EQUAL("foo", trim(" \tfoo"));
    ASSERT_STRING_EQUAL("foo", trim(" \tfoo\t "));
    ASSERT_STRING_EQUAL("foo\t bar", trim("foo\t bar"));
    ASSERT_STRING_EQUAL("foo\t bar", trim(" \tfoo\t bar"));
    ASSERT_STRING_EQUAL("foo\t bar", trim(" \tfoo\t bar \t"));
}

void CLASS::testSplit()
{
    StringVectorPtr fields = split("a,b,c", ",");
    CPPUNIT_ASSERT_EQUAL((size_t) 3, fields->size());
    ASSERT_STRING_EQUAL("a", (*fields)[0]);
    ASSERT_STRING_EQUAL("b", (*fields)[1]);
    ASSERT_STRING_EQUAL("c", (*fields)[2]);
}

void CLASS::testSplitWithLimit()
{
    StringVectorPtr fields = split("a,b,c", ",", 2);
    CPPUNIT_ASSERT_EQUAL((size_t) 2, fields->size());
    ASSERT_STRING_EQUAL("a", (*fields)[0]);
    ASSERT_STRING_EQUAL("b,c", (*fields)[1]);
}

void CLASS::testSplitNoEmpties()
{
    StringVectorPtr fields =
        split_no_empties("\r\nField1 = Value1\r\nField2 = Value2\r\n\r\n",
                         "\r\n");
    CPPUNIT_ASSERT_EQUAL((size_t) 2, fields->size());
    ASSERT_STRING_EQUAL("Field1 = Value1", (*fields)[0]);
    ASSERT_STRING_EQUAL("Field2 = Value2", (*fields)[1]);
}

void CLASS::testToUpper()
{
    ASSERT_STRING_EQUAL("FOO BAR", toUpper("Foo Bar"));
}

void CLASS::testStartsWith()
{
    CPPUNIT_ASSERT(startsWith("foo bar", "foo"));
    CPPUNIT_ASSERT(!startsWith("foo bar", " foo"));
    CPPUNIT_ASSERT(!startsWith(" foo bar", "foo"));
    CPPUNIT_ASSERT(!startsWith("foo", "foo bar"));
}

void CLASS::testUrlEncode()
{
    ASSERT_STRING_EQUAL("foo%2bbar", urlEncode("foo+bar"));
}

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
    CPPUNIT_TEST(testVectorEqualsWithPointers);
    CPPUNIT_TEST(testVectorEqualsWithSharedPointers);
    CPPUNIT_TEST(testSplitField);
    CPPUNIT_TEST(testUrlEncode);
    CPPUNIT_TEST(testJoinStrings);
    CPPUNIT_TEST(testReadIntoString);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testVectorEqualsWithPointers();
    void testVectorEqualsWithSharedPointers();
    void testSplitField();
    void testUrlEncode();
    void testJoinStrings();
    void testReadIntoString();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testVectorEqualsWithPointers()
{
    TestObject object1_1(1);
    TestObject object1_2(2);
    TestObject object1_3(3);
    TestObject object1_4(4);

    vector<TestObject *> v1;
    v1.push_back(&object1_1);
    v1.push_back(&object1_2);
    v1.push_back(&object1_3);
    
    TestObject object2_1(1);
    TestObject object2_2(2);
    TestObject object2_3(3);

    vector<TestObject *> v2;
    v2.push_back(&object2_1);
    v2.push_back(&object2_2);
    v2.push_back(&object2_3);
    
    TestObject object3_1(1);
    TestObject object3_2(2);
    TestObject object3_3(3);
    
    vector<TestObject *> v3;
    v3.push_back(&object3_3);
    v3.push_back(&object3_2);
    v3.push_back(&object3_1);
    
    TestObject object4_1(1);
    TestObject object4_2(2);
    TestObject object4_3(3);
    TestObject object4_4(4);

    vector<TestObject *> v4;
    v4.push_back(&object4_1);
    v4.push_back(&object4_2);
    v4.push_back(&object4_3);
    v4.push_back(&object4_4);
    
    TestObject object5_1(1);
    TestObject object5_2(2);

    vector<TestObject *> v5;
    v5.push_back(&object5_1);
    v5.push_back(&object5_2);
    
    CPPUNIT_ASSERT(VectorEquals(v1, v2));
    CPPUNIT_ASSERT(!VectorEquals(v1, v3));
    CPPUNIT_ASSERT(!VectorEquals(v1, v4));
    CPPUNIT_ASSERT(!VectorEquals(v1, v4));
}

void CLASS::testVectorEqualsWithSharedPointers()
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
    ASSERT_STRING_EQUAL("-%3d-%2b-%3f-%26-(-)-:-%2525-",
                        urlEncode("-=-+-?-&-(-)-:-%25-"));
}

void CLASS::testJoinStrings()
{
    ASSERT_STRING_EQUAL("foo", join("foo", "", ":"));
    ASSERT_STRING_EQUAL("bar", join("", "bar", ":"));
    ASSERT_STRING_EQUAL("foo:bar", join("foo", "bar", ":"));
}

void CLASS::testReadIntoString()
{
    iostreamPtr stream(new stringstream("string data"));
    string aString;
    readIntoString(stream, aString);
    ASSERT_STRING_EQUAL("string data", aString);
}

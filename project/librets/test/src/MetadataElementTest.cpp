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
#include "testUtil.h"
#include "TestMetadataElement.h"

using namespace librets;
using namespace std;
namespace b = boost;

#define CLASS MetadataElementTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testGetIntAttribute);
    CPPUNIT_TEST(testGetBoolAttribute);
    CPPUNIT_TEST(testGetPath);
    CPPUNIT_TEST(testIdEqualPred);
    CPPUNIT_TEST(testGetAttributeNames);
    CPPUNIT_TEST_SUITE_END();
    
  protected:
    void testGetIntAttribute();
    void testGetBoolAttribute();
    void testGetPath();
    void testIdEqualPred();
    void testGetAttributeNames();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testGetIntAttribute()
{
    TestMetadataElement element;
    element.SetAttribute("field1", "5");
    element.SetAttribute("field2", "");
    element.SetAttribute("field3", " ");
    ASSERT_EQUAL(5, element.GetIntAttribute("field1"));
    ASSERT_EQUAL(0, element.GetIntAttribute("field2"));
    ASSERT_EQUAL(0, element.GetIntAttribute("field3"));
    ASSERT_EQUAL(0, element.GetIntAttribute("field4"));
}

void CLASS::testGetBoolAttribute()
{
    TestMetadataElement element;
    element.SetAttribute("field1", "1");
    element.SetAttribute("field2", "0");
    element.SetAttribute("field3", "true");
    element.SetAttribute("field4", "false");
    element.SetAttribute("field5", "True");
    element.SetAttribute("field6", "False");
    ASSERT_EQUAL(true,  element.GetBoolAttribute("field1"));
    ASSERT_EQUAL(false, element.GetBoolAttribute("field2"));
    ASSERT_EQUAL(true,  element.GetBoolAttribute("field3"));
    ASSERT_EQUAL(false, element.GetBoolAttribute("field4"));
    ASSERT_EQUAL(true,  element.GetBoolAttribute("field5"));
    ASSERT_EQUAL(false, element.GetBoolAttribute("field6"));
    ASSERT_EQUAL(false, element.GetBoolAttribute("field7"));
}

void CLASS::testGetPath()
{
    TestMetadataElement element;
    element.SetLevel("Property");
    ASSERT_STRING_EQUAL("", element.GetPath());

    element.SetAttribute("ID", "RES");
    ASSERT_STRING_EQUAL("Property:RES", element.GetPath());
    
    element.SetLevel("");
    ASSERT_STRING_EQUAL("RES", element.GetPath());
}

void CLASS::testIdEqualPred()
{
    TestMetadataElement element1;
    element1.SetAttribute("ID", "1");

    TestMetadataElement element2;
    element2.SetAttribute("ID", "2");

    TestMetadataElement element3;
    element3.SetAttribute("ID", "1");
    
    MetadataElementIdEqual idOne("1");
    CPPUNIT_ASSERT(idOne(&element1));
    CPPUNIT_ASSERT(!idOne(&element2));
    CPPUNIT_ASSERT(idOne(&element3));
    
    MetadataElementIdEqual idTwo("2");
    CPPUNIT_ASSERT(!idTwo(&element1));
    CPPUNIT_ASSERT(idTwo(&element2));
    CPPUNIT_ASSERT(!idTwo(&element3));
    
    MetadataElementIdEqual idThree("3");
    CPPUNIT_ASSERT(!idThree(&element1));
    CPPUNIT_ASSERT(!idThree(&element2));
    CPPUNIT_ASSERT(!idThree(&element3));
}

void CLASS::testGetAttributeNames()
{
    TestMetadataElement element;
    element.SetAttribute("field1", "foo");
    element.SetAttribute("field2", "bar");
    
    StringVector names = element.GetAttributeNames();
    set<string> setOfNames(names.begin(), names.end());
    ASSERT_EQUAL(size_t(2), setOfNames.size());
    ASSERT_EQUAL(size_t(1), setOfNames.count("field1"));
    ASSERT_EQUAL(size_t(1), setOfNames.count("field2"));
}
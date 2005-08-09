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
    CPPUNIT_TEST_SUITE_END();
    
protected:
    void testGetIntAttribute();
    void testGetBoolAttribute();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testGetIntAttribute()
{
    TestMetadataElement element;
    element.SetAttribute("field1", "5");
    element.SetAttribute("field2", "");
    ASSERT_EQUAL(5, element.GetIntAttribute("field1"));
    ASSERT_EQUAL(0, element.GetIntAttribute("field2"));
    ASSERT_EQUAL(0, element.GetIntAttribute("field3"));
}

void CLASS::testGetBoolAttribute()
{
    TestMetadataElement element;
    element.SetAttribute("field1", "1");
    element.SetAttribute("field2", "0");
    element.SetAttribute("field3", "true");
    element.SetAttribute("field4", "false");
    ASSERT_EQUAL(true,  element.GetBoolAttribute("field1"));
    ASSERT_EQUAL(false, element.GetBoolAttribute("field2"));
//    ASSERT_EQUAL(true,  element.GetBoolAttribute("field3"));
//    ASSERT_EQUAL(false, element.GetBoolAttribute("field4"));
}
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
#include "librets/RetsXmlAttribute.h"
#include "testUtil.h"

using namespace librets;
using namespace std;

typedef vector<RetsXmlEventPtr> EventVector;

#define CLASS RetsXmlAttributeTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testPrint);
    CPPUNIT_TEST(testEqual);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testPrint();
    void testEqual();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testPrint()
{
    RetsXmlAttribute attribute("foo", "bar");
    ostringstream actual;
    actual << attribute;
    ASSERT_STRING_EQUAL("RetsXmlAttribute[foo=bar]", actual.str());
}

void CLASS::testEqual()
{
    RetsXmlAttribute attribute1("foo", "bar");
    RetsXmlAttribute attribute2("bar", "foo");
    RetsXmlAttribute attribute3("foo", "bar");
    CPPUNIT_ASSERT_EQUAL(attribute1, attribute3);
    CPPUNIT_ASSERT_EQUAL(attribute3, attribute1);
    bool equal = (attribute1 == attribute2);
    CPPUNIT_ASSERT(!equal);
    equal = (attribute2 == attribute1);
    CPPUNIT_ASSERT(!equal);
}

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
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlAttribute.h"
#include "testUtil.h"

using namespace librets;
using namespace std;

typedef vector<RetsXmlEventPtr> EventVector;

#define CLASS RetsXmlStartElementEventTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testPrint);
    CPPUNIT_TEST(testEqual);
    CPPUNIT_TEST(testGetAttributeValue);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testPrint();
    void testEqual();
    void testGetAttributeValue();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testPrint()
{
    RetsXmlStartElementEvent event;
    event.SetName("start");
    RetsXmlAttributePtr attribute(new RetsXmlAttribute("foo", "bar"));
    event.AddAttribute(attribute);
    attribute.reset(new RetsXmlAttribute("Donald", "Duck"));
    event.AddAttribute(attribute);
    ostringstream actual;
    actual << event;
    ASSERT_STRING_EQUAL(
        "XML start element: <start>, attributes [foo=bar, Donald=Duck]",
        actual.str());
}

void CLASS::testEqual()
{
    RetsXmlAttributePtr attribute;
    RetsXmlStartElementEvent event1;
    event1.SetName("start");
    attribute.reset(new RetsXmlAttribute("foo", "bar"));
    event1.AddAttribute(attribute);
    attribute.reset(new RetsXmlAttribute("Donald", "Duck"));
    event1.AddAttribute(attribute);

    RetsXmlStartElementEvent event2;
    event2.SetName("name2");
    attribute.reset(new RetsXmlAttribute("foo", "bar"));
    event2.AddAttribute(attribute);
    attribute.reset(new RetsXmlAttribute("Donald", "Duck"));
    event2.AddAttribute(attribute);

    RetsXmlStartElementEvent event3;
    event3.SetName("start");
    attribute.reset(new RetsXmlAttribute("foo", "bar"));
    event3.AddAttribute(attribute);
    attribute.reset(new RetsXmlAttribute("Donald", "Duck"));
    event3.AddAttribute(attribute);

    RetsXmlStartElementEvent event4;
    event4.SetName("start");
    attribute.reset(new RetsXmlAttribute("foo", "bar"));
    event4.AddAttribute(attribute);
    attribute.reset(new RetsXmlAttribute("Mickey", "Mouse"));
    event4.AddAttribute(attribute);

    RetsXmlStartElementEvent event5;
    event4.SetName("start");

    bool equal;
    equal = (event1 == event2);
    CPPUNIT_ASSERT(!equal);
    equal = (event1 == event3);
    CPPUNIT_ASSERT(equal);
    equal = (event1 == event4);
    CPPUNIT_ASSERT(!equal);
    equal = (event1 == event5);
    CPPUNIT_ASSERT(!equal);
    equal = (event3 == event1);
    CPPUNIT_ASSERT(equal);
}

void CLASS::testGetAttributeValue()
{
    RetsXmlStartElementEvent event;
    event.SetName("start");
    RetsXmlAttributePtr attribute(new RetsXmlAttribute("foo", "bar"));
    event.AddAttribute(attribute);
    attribute.reset(new RetsXmlAttribute("Donald", "Duck"));
    event.AddAttribute(attribute);

    ASSERT_STRING_EQUAL("bar", event.GetAttributeValue("foo"));
    ASSERT_STRING_EQUAL("Duck", event.GetAttributeValue("Donald"));
    ASSERT_STRING_EQUAL("", event.GetAttributeValue("name"));
}

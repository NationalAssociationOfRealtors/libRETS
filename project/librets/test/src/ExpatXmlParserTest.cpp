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
#include <vector>
#include <sstream>
#include "testUtil.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/RetsXmlEndDocumentEvent.h"
#include "librets/RetsXmlAttribute.h"

using namespace librets;
using namespace std;

#define CLASS ExpatXmlParserTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(test512ByteDocument);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testSimple();
    void test512ByteDocument();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testSimple()
{
    string xml =
        "<one attr=\"value\">foo"
        "  <two attr1=\"value1\" attr2=\"value2\">bar</two>"
        "</one>\n";
    
    RetsXmlEventList expected;
    
    RetsXmlStartElementEventPtr start;
    RetsXmlAttributePtr attribute;
    RetsXmlEndElementEventPtr end;
    RetsXmlTextEventPtr text;

    start.reset(new RetsXmlStartElementEvent());
    start->SetName("one");
    attribute.reset(new RetsXmlAttribute("attr", "value"));
    start->AddAttribute(attribute);
    expected.push_back(start);

    text.reset(new RetsXmlTextEvent());
    text->AppendText("foo  ");
    expected.push_back(text);

    start.reset(new RetsXmlStartElementEvent());
    start->SetName("two");
    attribute.reset(new RetsXmlAttribute("attr1", "value1"));
    start->AddAttribute(attribute);
    attribute.reset(new RetsXmlAttribute("attr2", "value2"));
    start->AddAttribute(attribute);
    expected.push_back(start);

    text.reset(new RetsXmlTextEvent());
    text->AppendText("bar");
    expected.push_back(text);

    end.reset(new RetsXmlEndElementEvent());
    end->SetName("two");
    expected.push_back(end);

    end.reset(new RetsXmlEndElementEvent());
    end->SetName("one");
    expected.push_back(end);
    
    RetsXmlEndDocumentEventPtr endDocument(new RetsXmlEndDocumentEvent());
    expected.push_back(endDocument);

    RetsXmlParserPtr xmlParser(new ExpatXmlParser(xml));
    RetsXmlEventListPtr actual = xmlParser->GetEventList();

    ASSERT_VECTOR_EQUAL(expected, *actual);
}

void CLASS::test512ByteDocument()
{
    string startTag = "<tag>";
    string endTag = "</tag>";
    int contentSize = 512 - startTag.length() - endTag.length(); 
    string content(contentSize, 'a');
    string xml = startTag + content + endTag;

    RetsXmlEventList expected;
    
    RetsXmlStartElementEventPtr start;
    RetsXmlTextEventPtr text;
    RetsXmlEndElementEventPtr end;
    
    start.reset(new RetsXmlStartElementEvent());
    start->SetName("tag");
    expected.push_back(start);
    
    text.reset(new RetsXmlTextEvent());
    text->AppendText(content);
    expected.push_back(text);
    
    end.reset(new RetsXmlEndElementEvent());
    end->SetName("tag");
    expected.push_back(end);
    
    RetsXmlEndDocumentEventPtr endDocument(new RetsXmlEndDocumentEvent());
    expected.push_back(endDocument);
    
    RetsXmlParserPtr xmlParser(new ExpatXmlParser(xml));
    RetsXmlEventListPtr actual = xmlParser->GetEventList();
    ASSERT_VECTOR_EQUAL(expected, *actual);
}

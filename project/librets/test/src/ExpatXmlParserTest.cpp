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

#include <boost/algorithm/string.hpp>
#include <boost/version.hpp>
#include <cppunit/extensions/HelperMacros.h>
#include <vector>
#include <sstream>
#include "testUtil.h"
#include "librets/RetsException.h"
#include "librets/RetsReplyException.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/RetsXmlEndDocumentEvent.h"
#include "librets/RetsXmlAttribute.h"

namespace b = boost;
namespace ba = boost::algorithm;

using namespace librets;
using namespace std;

#define CLASS ExpatXmlParserTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testSimple);
    CPPUNIT_TEST(test512ByteDocument);
    CPPUNIT_TEST(testPayload);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testSimple();
    void test512ByteDocument();
    void testPayload();
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

std::string payload =
"<?xml version=\"1.0\"?>"
"<RETS ReplyCode=\"0\""
"  ReplyText=\"Operation Successful Reference ID: 6887b2d8-bb8c-49bf-9d83-1e92144426ab\">"
"  <COUNT Records=\"269145\" />"
"  <Properties>"
"    <q1:Property xmlns:q1=\"http://www.reso.org/DataDictionary1.1/Property\">"
"      <BedroomsTotal>3</BedroomsTotal>"
"      <BuyerOfficeMlsId>FOO</BuyerOfficeMlsId>"
"      <CloseDate>2001-12-11</CloseDate>"
"      <ClosePrice>125000</ClosePrice>"
"      <ClosingTerms>"
"        <LookupValue>FHA</LookupValue>"
"      </ClosingTerms>"
"      <DaysOnMarket>143</DaysOnMarket>"
"      <ListAgentFullName>Schmoe, Joe</ListAgentFullName>"
"      <ListingContractDate>2001-11-19</ListingContractDate>"
"      <ListingId>1234567</ListingId>"
"      <ListOfficeMlsId>BAR</ListOfficeMlsId>"
"      <ListPrice>0</ListPrice>"
"      <MLSAreaMajor>101 - North Las Vegas</MLSAreaMajor>"
"      <MlsStatus>History</MlsStatus>"
"      <ModificationTimestamp>"
"      2004-09-28T03:37:04</ModificationTimestamp>"
"      <OriginalEntryTimestamp>"
"      2001-07-23T16:55:00</OriginalEntryTimestamp>"
"      <OriginalListPrice>129000</OriginalListPrice>"
"      <ParcelNumber>123-45-678-900</ParcelNumber>"
"      <PhotosCount>1</PhotosCount>"
"      <PostalCode>11111</PostalCode>"
"      <PreviousListPrice>125000</PreviousListPrice>"
"      <PropertyType>Single Family Res</PropertyType>"
"      <StreetName>HOMEOWNER LANE</StreetName>"
"      <StreetNumber>2222</StreetNumber>"
"      <TaxBlock>20</TaxBlock>"
"      <UnparsedAddress>2222 HOMEOWNER LANE</UnparsedAddress>"
"    </q1:Property>"
"  </Properties>"
"  <MAXROWS />"
"</RETS>";

void CLASS::testPayload()
{
    RetsXmlEventList expected;

    RetsXmlParserPtr mXmlParser(new ExpatXmlParser(payload));

    RetsXmlEndElementEventPtr endEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlTextEventPtr textEvent;
    
    startEvent = mXmlParser->AssertNextIsStartEvent("RETS");
    ASSERT_STRING_EQUAL("RETS", startEvent->GetName());
    startEvent = mXmlParser->AssertNextIsStartEvent("COUNT");
    ASSERT_STRING_EQUAL("COUNT", startEvent->GetName());
    endEvent = mXmlParser->AssertNextIsEndEvent("COUNT");
    ASSERT_STRING_EQUAL("COUNT", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("Properties");
    ASSERT_STRING_EQUAL("Properties", startEvent->GetName());
    startEvent = mXmlParser->AssertNextIsStartEvent("q1:Property");
    ASSERT_STRING_EQUAL("q1:Property", startEvent->GetName());
    startEvent = mXmlParser->AssertNextIsStartEvent("BedroomsTotal");
    ASSERT_STRING_EQUAL("BedroomsTotal", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("3");
    ASSERT_STRING_EQUAL("3", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("BedroomsTotal");
    ASSERT_STRING_EQUAL("BedroomsTotal", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("BuyerOfficeMlsId");
    ASSERT_STRING_EQUAL("BuyerOfficeMlsId", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("FOO");
    ASSERT_STRING_EQUAL("FOO", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("BuyerOfficeMlsId");
    ASSERT_STRING_EQUAL("BuyerOfficeMlsId", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("CloseDate");
    ASSERT_STRING_EQUAL("CloseDate", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("2001-12-11");
    ASSERT_STRING_EQUAL("2001-12-11", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("CloseDate");
    ASSERT_STRING_EQUAL("CloseDate", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ClosePrice");
    ASSERT_STRING_EQUAL("ClosePrice", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("125000");
    ASSERT_STRING_EQUAL("125000", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ClosePrice");
    ASSERT_STRING_EQUAL("ClosePrice", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ClosingTerms");
    ASSERT_STRING_EQUAL("ClosingTerms", startEvent->GetName());
    startEvent = mXmlParser->AssertNextIsStartEvent("LookupValue");
    ASSERT_STRING_EQUAL("LookupValue", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("FHA");
    ASSERT_STRING_EQUAL("FHA", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("LookupValue");
    ASSERT_STRING_EQUAL("LookupValue", endEvent->GetName());
    
    endEvent = mXmlParser->AssertNextIsEndEvent("ClosingTerms");
    ASSERT_STRING_EQUAL("ClosingTerms", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("DaysOnMarket");
    ASSERT_STRING_EQUAL("DaysOnMarket", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("143");
    ASSERT_STRING_EQUAL("143", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("DaysOnMarket");
    ASSERT_STRING_EQUAL("DaysOnMarket", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ListAgentFullName");
    ASSERT_STRING_EQUAL("ListAgentFullName", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("Schmoe, Joe");
    ASSERT_STRING_EQUAL("Schmoe, Joe", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ListAgentFullName");
    ASSERT_STRING_EQUAL("ListAgentFullName", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ListingContractDate");
    ASSERT_STRING_EQUAL("ListingContractDate", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("2001-11-19");
    ASSERT_STRING_EQUAL("2001-11-19", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ListingContractDate");
    ASSERT_STRING_EQUAL("ListingContractDate", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ListingId");
    ASSERT_STRING_EQUAL("ListingId", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("1234567");
    ASSERT_STRING_EQUAL("1234567", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ListingId");
    ASSERT_STRING_EQUAL("ListingId", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ListOfficeMlsId");
    ASSERT_STRING_EQUAL("ListOfficeMlsId", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("BAR");
    ASSERT_STRING_EQUAL("BAR", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ListOfficeMlsId");
    ASSERT_STRING_EQUAL("ListOfficeMlsId", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ListPrice");
    ASSERT_STRING_EQUAL("ListPrice", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("0");
    ASSERT_STRING_EQUAL("0", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ListPrice");
    ASSERT_STRING_EQUAL("ListPrice", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("MLSAreaMajor");
    ASSERT_STRING_EQUAL("MLSAreaMajor", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("101 - North Las Vegas");
    ASSERT_STRING_EQUAL("101 - North Las Vegas", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("MLSAreaMajor");
    ASSERT_STRING_EQUAL("MLSAreaMajor", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("MlsStatus");
    ASSERT_STRING_EQUAL("MlsStatus", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("History");
    ASSERT_STRING_EQUAL("History", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("MlsStatus");
    ASSERT_STRING_EQUAL("MlsStatus", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ModificationTimestamp");
    ASSERT_STRING_EQUAL("ModificationTimestamp", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("2004-09-28T03:37:04");
    ASSERT_STRING_EQUAL("2004-09-28T03:37:04", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ModificationTimestamp");
    ASSERT_STRING_EQUAL("ModificationTimestamp", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("OriginalEntryTimestamp");
    ASSERT_STRING_EQUAL("OriginalEntryTimestamp", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("2001-07-23T16:55:00");
    ASSERT_STRING_EQUAL("2001-07-23T16:55:00", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("OriginalEntryTimestamp");
    ASSERT_STRING_EQUAL("OriginalEntryTimestamp", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("OriginalListPrice");
    ASSERT_STRING_EQUAL("OriginalListPrice", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("129000");
    ASSERT_STRING_EQUAL("129000", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("OriginalListPrice");
    ASSERT_STRING_EQUAL("OriginalListPrice", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("ParcelNumber");
    ASSERT_STRING_EQUAL("ParcelNumber", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("123-45-678-900");
    ASSERT_STRING_EQUAL("123-45-678-900", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("ParcelNumber");
    ASSERT_STRING_EQUAL("ParcelNumber", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("PhotosCount");
    ASSERT_STRING_EQUAL("PhotosCount", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("1");
    ASSERT_STRING_EQUAL("1", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("PhotosCount");
    ASSERT_STRING_EQUAL("PhotosCount", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("PostalCode");
    ASSERT_STRING_EQUAL("PostalCode", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("11111");
    ASSERT_STRING_EQUAL("11111", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("PostalCode");
    ASSERT_STRING_EQUAL("PostalCode", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("PreviousListPrice");
    ASSERT_STRING_EQUAL("PreviousListPrice", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("125000");
    ASSERT_STRING_EQUAL("125000", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("PreviousListPrice");
    ASSERT_STRING_EQUAL("PreviousListPrice", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("PropertyType");
    ASSERT_STRING_EQUAL("PropertyType", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("Single Family Res");
    ASSERT_STRING_EQUAL("Single Family Res", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("PropertyType");
    ASSERT_STRING_EQUAL("PropertyType", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("StreetName");
    ASSERT_STRING_EQUAL("StreetName", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("HOMEOWNER LANE");
    ASSERT_STRING_EQUAL("HOMEOWNER LANE", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("StreetName");
    ASSERT_STRING_EQUAL("StreetName", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("StreetNumber");
    ASSERT_STRING_EQUAL("StreetNumber", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("2222");
    ASSERT_STRING_EQUAL("2222", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("StreetNumber");
    ASSERT_STRING_EQUAL("StreetNumber", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("TaxBlock");
    ASSERT_STRING_EQUAL("TaxBlock", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("20");
    ASSERT_STRING_EQUAL("20", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("TaxBlock");
    ASSERT_STRING_EQUAL("TaxBlock", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("UnparsedAddress");
    ASSERT_STRING_EQUAL("UnparsedAddress", startEvent->GetName());
    textEvent = mXmlParser->AssertNextIsTextEvent("2222 HOMEOWNER LANE");
    ASSERT_STRING_EQUAL("2222 HOMEOWNER LANE", ba::trim_copy(textEvent->GetText()));
    endEvent = mXmlParser->AssertNextIsEndEvent("UnparsedAddress");
    ASSERT_STRING_EQUAL("UnparsedAddress", endEvent->GetName());
    
    endEvent = mXmlParser->AssertNextIsEndEvent("q1:Property");
    ASSERT_STRING_EQUAL("q1:Property", endEvent->GetName());
    
    endEvent = mXmlParser->AssertNextIsEndEvent("Properties");
    ASSERT_STRING_EQUAL("Properties", endEvent->GetName());
    
    startEvent = mXmlParser->AssertNextIsStartEvent("MAXROWS");
    ASSERT_STRING_EQUAL("MAXROWS", startEvent->GetName());
    endEvent = mXmlParser->AssertNextIsEndEvent("MAXROWS");
    ASSERT_STRING_EQUAL("MAXROWS", endEvent->GetName());
    
    endEvent = mXmlParser->AssertNextIsEndEvent("RETS");
    ASSERT_STRING_EQUAL("RETS", endEvent->GetName());
}

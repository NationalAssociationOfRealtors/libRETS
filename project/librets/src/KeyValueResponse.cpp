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

#include <sstream>
#include <boost/algorithm/string.hpp>
#include "librets/KeyValueResponse.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/util.h"
#include "librets/RetsException.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostringstream;
namespace b = boost;
namespace ba = boost::algorithm;

#define CLASS KeyValueResponse

CLASS::~CLASS()
{
}

void CLASS::AssertEquals(const string & expected, const string & actual)
{
    if (expected != actual)
    {
        ostringstream message;
        message << "Not equals, expected: <" << expected << ">, actual: <"
                << actual << ">";
        throw RetsException(message.str());
    }
}

void CLASS::AssertEquals(int expected, int actual)
{
    if (expected != actual)
    {
        ostringstream message;
        message << "Not equals, expected: <" << expected << ">, actual: <"
            << actual << ">";
        throw RetsException(message.str());
    }
}

void CLASS::Parse(istreamPtr inputStream)
{
    Parse(inputStream, RETS_1_5);
}

void CLASS::Parse(istreamPtr inputStream, RetsVersion retsVersion)
{
    RetsXmlParserPtr xmlParser(new ExpatXmlParser(inputStream));
    RetsXmlEventListPtr eventList = xmlParser->GetEventListSkippingEmptyText();
    RetsXmlTextEventPtr bodyEvent = GetBodyEvent(eventList, retsVersion);
    if (bodyEvent)
    {
        ParseBody(bodyEvent->GetText());
    }
    ParsingFinished();
}

RetsXmlTextEventPtr CLASS::GetBodyEventFromStandardResponse(
    RetsXmlEventListPtr eventList)
{
    RetsXmlTextEventPtr bodyEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    
    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
    AssertEquals("RETS", startEvent->GetName());
    AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(1));
    AssertEquals("RETS-RESPONSE", startEvent->GetName());
    bodyEvent = RetsXmlParser::AssertTextEvent(eventList->at(2));
    endEvent = RetsXmlParser::AssertEndEvent(eventList->at(3));
    AssertEquals("RETS-RESPONSE", endEvent->GetName());
    endEvent = RetsXmlParser::AssertEndEvent(eventList->at(4));
    AssertEquals("RETS", endEvent->GetName());
    AssertEquals(5, eventList->size());
    
    return bodyEvent;
}

RetsXmlTextEventPtr CLASS::GetBodyEventFromResponseWithNoRetsResponse(
    RetsXmlEventListPtr eventList)
{
    RetsXmlTextEventPtr bodyEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    
    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
    AssertEquals("RETS", startEvent->GetName());
    AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    bodyEvent = RetsXmlParser::AssertTextEvent(eventList->at(1));
    endEvent = RetsXmlParser::AssertEndEvent(eventList->at(2));
    AssertEquals("RETS", endEvent->GetName());
    AssertEquals(3, eventList->size());
    
    return bodyEvent;
}

RetsXmlTextEventPtr CLASS::GetBodyEventFromShortResponse(
    RetsXmlEventListPtr eventList)
{
    RetsXmlTextEventPtr bodyEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    
    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
    AssertEquals("RETS", startEvent->GetName());
    AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    endEvent = RetsXmlParser::AssertEndEvent(eventList->at(1));
    AssertEquals("RETS", endEvent->GetName());
    AssertEquals(2, eventList->size());
    
    return bodyEvent;
}

void CLASS::ParseBody(string body)
{
    StringVector lines;
    ba::split(lines, body, ba::is_any_of("\n"));
    StringVector::const_iterator line;
    for (line = lines.begin(); line != lines.end(); line++)
    {
        if (line->empty())
        {
            continue;
        }
        string key;
        string value;
        if(!splitField(*line, "=", key, value))
        {
            throw RetsException("Invalid key value pair <" + *line + ">");
        }
        ba::trim(key);
        ba::to_upper(key);
        ba::trim(value);
        mValues[key] = value;
    }
}

string CLASS::GetValue(string key) const
{
    ba::to_upper(key);
    StringMap::const_iterator value = mValues.find(key);
    if (value == mValues.end())
    {
        return "";
    }
    return value->second;
}

void CLASS::ParsingFinished()
{
}

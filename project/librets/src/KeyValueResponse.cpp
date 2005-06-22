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

RetsXmlEventPtr CLASS::getNextSkippingEmptyText(RetsXmlParserPtr xmlParser)
{
    if (!xmlParser->HasNext())
    {
        throw RetsException("No more XML events");
    }

    RetsXmlEventPtr event = xmlParser->GetNextEvent();
    RetsXmlTextEventPtr textEvent =
        b::dynamic_pointer_cast<RetsXmlTextEvent>(event);
    if (!textEvent)
    {
        return event;
    }

    string text = textEvent->GetText();
    if (isEmpty(text))
    {
        return getNextSkippingEmptyText(xmlParser);
    }
    else
    {
        return event;
    }
}

RetsXmlStartElementEventPtr CLASS::assertNextIsStart(
    RetsXmlParserPtr xmlParser)
{
    RetsXmlEventPtr event = getNextSkippingEmptyText(xmlParser);
    RetsXmlStartElementEventPtr startEvent =
        b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
    if (!startEvent)
    {
        ostringstream message;
        message << "Event is not a start event: " << event;
        throw RetsException(message.str());
    }
    return startEvent;
}

RetsXmlEndElementEventPtr CLASS::assertNextIsEnd(RetsXmlParserPtr xmlParser)
{
    RetsXmlEventPtr event = getNextSkippingEmptyText(xmlParser);
    RetsXmlEndElementEventPtr endEvent =
        b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
    if (!endEvent)
    {
        ostringstream message;
        message << "Event is not an end event: " << event;
        throw RetsException(message.str());
    }
    return endEvent;
}

RetsXmlTextEventPtr CLASS::assertNextIsText(RetsXmlParserPtr xmlParser)
{
    RetsXmlEventPtr event = getNextSkippingEmptyText(xmlParser);
    RetsXmlTextEventPtr textEvent =
        b::dynamic_pointer_cast<RetsXmlTextEvent>(event);
    if (!textEvent)
    {
        ostringstream message;
        message << "Event is not a text event: " << event;
        throw RetsException(message.str());
    }
    return textEvent;
}

void CLASS::assertEquals(const string & expected, const string & actual)
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
    RetsXmlParserPtr xmlParser(new ExpatXmlParser(inputStream));
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    string body;

    startEvent = assertNextIsStart(xmlParser);
    assertEquals("RETS", startEvent->GetName());
    assertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    RetsXmlEventPtr event = getNextSkippingEmptyText(xmlParser);
    startEvent = b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
    if (!startEvent)
    {
        endEvent = b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
        if (!endEvent)
        {
            ostringstream message;
            message << "Event is not an end event: " << event;
            throw RetsException(message.str());
        }
        // Success
        return;
    }
    assertEquals("RETS-RESPONSE", startEvent->GetName());
    textEvent = assertNextIsText(xmlParser);
    body = textEvent->GetText();
    endEvent = assertNextIsEnd(xmlParser);
    assertEquals("RETS-RESPONSE", endEvent->GetName());
    endEvent = assertNextIsEnd(xmlParser);
    assertEquals("RETS", endEvent->GetName());
    
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
    ParsingFinished();
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

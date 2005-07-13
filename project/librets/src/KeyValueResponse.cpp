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
    Parse(inputStream, RETS_1_5);
}

void CLASS::Parse(istreamPtr inputStream, RetsVersion retsVersion)
{
    RetsXmlParserPtr xmlParser(new ExpatXmlParser(inputStream));
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    string body;

    startEvent = xmlParser->AssertNextIsStartEvent();
    assertEquals("RETS", startEvent->GetName());
    assertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    RetsXmlEventPtr event = xmlParser->GetNextSkippingEmptyText();
    endEvent = b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
    if (endEvent)
    {
        // Short response, no body
        return;
    }
    if (retsVersion != RETS_1_0)
    {
        startEvent = xmlParser->AssertStartEvent(event);
        assertEquals("RETS-RESPONSE", startEvent->GetName());
        textEvent = xmlParser->AssertNextIsTextEvent();
    }
    else
    {
        textEvent = xmlParser->AssertTextEvent(event);
    }
    body = textEvent->GetText();
    if (retsVersion != RETS_1_0)
    {
        endEvent = xmlParser->AssertNextIsEndEvent();
        assertEquals("RETS-RESPONSE", endEvent->GetName());
    }
    endEvent = xmlParser->AssertNextIsEndEvent();
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

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
#include "librets/RetsXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/RetsException.h"
#include "librets/ExpatXmlParser.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostringstream;
namespace b = boost;

#define CLASS RetsXmlParser

RetsXmlParserPtr RetsXmlParser::CreateDefault(istreamPtr inputStream)
{
    RetsXmlParserPtr xmlParser(new ExpatXmlParser(inputStream));
    return xmlParser;
}


RetsXmlParser::~RetsXmlParser()
{
}

RetsXmlEventPtr CLASS::GetNextSkippingEmptyText()
{
    RetsXmlParser * xmlParser = this;
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
        return GetNextSkippingEmptyText();
    }
    else
    {
        return event;
    }
}

RetsXmlStartElementEventPtr CLASS::AssertNextIsStartEvent(string prefix)
{
    RetsXmlEventPtr event = GetNextSkippingEmptyText();
    RetsXmlStartElementEventPtr startEvent =
        b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
    if (!startEvent)
    {
        ostringstream message;
        message << prefix << "Event is not a start event: " << event;
        throw RetsException(message.str());
    }
    return startEvent;
}

RetsXmlEndElementEventPtr CLASS::AssertNextIsEndEvent(string prefix)
{
    RetsXmlEventPtr event = GetNextSkippingEmptyText();
    RetsXmlEndElementEventPtr endEvent =
        b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
    if (!endEvent)
    {
        ostringstream message;
        message << prefix <<  "Event is not an end event: " << event;
        throw RetsException(message.str());
    }
    return endEvent;
}

RetsXmlTextEventPtr CLASS::AssertNextIsTextEvent(string prefix)
{
    return AssertTextEvent(GetNextSkippingEmptyText());
}

RetsXmlTextEventPtr CLASS::AssertTextEvent(RetsXmlEventPtr event, string prefix)
{
    RetsXmlTextEventPtr textEvent =
        b::dynamic_pointer_cast<RetsXmlTextEvent>(event);
    if (!textEvent)
    {
        ostringstream message;
        message << prefix << "Event is not a text event: " << event;
        throw RetsException(message.str());
    }
    return textEvent;
}

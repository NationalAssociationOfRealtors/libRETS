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
using std::istringstream;
using std::ostringstream;
namespace b = boost;
namespace ba = boost::algorithm;

#define CLASS KeyValueResponse

CLASS::CLASS()
{
    mRetsReplyCode = 0;
    mRetsReplyText = "";
}

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

void CLASS::AssertEventListSize(int expected,
                                RetsXmlEventListPtr eventList)
{
    int actual = eventList->size();
    if (expected != actual)
    {
        ostringstream message;
        message << "Size not equals, expected: <" << expected << ">, actual: <"
            << actual << ">, event list: " << Output(*eventList);
        throw RetsException(message.str());
    }
}

void CLASS::AssertNotEquals(const string & expected, const string & actual)
{
    if (expected == actual)
    {
        ostringstream message;
        message << "Not equals, expected: <" << expected << ">, actual: <"
        << actual << ">";
        throw RetsException(message.str());
    }
}

void CLASS::Parse(istreamPtr inputStream, RetsVersion retsVersion)
{
    Parse(inputStream, retsVersion, RETS_XML_DEFAULT_ENCODING);
}

void CLASS::Parse(istreamPtr inputStream, RetsVersion retsVersion, EncodingType encoding)
{
    RetsXmlParserPtr xmlParser(new ExpatXmlParser(inputStream,
                        (encoding == RETS_XML_ISO_ENCODING
                           ? "iso-8859-1"
                           : (encoding == RETS_XML_UTF8_ENCODING
                                ? "UTF-8"
                                : "US-ASCII"))));
                           
    mRetsVersion = retsVersion;
    RetsXmlEventListPtr eventList = xmlParser->GetEventListSkippingEmptyText();
    RetsXmlTextEventPtr bodyEvent = GetBodyEvent(eventList, retsVersion);
    if (bodyEvent)
    {
        ParseBody(bodyEvent->GetText());
    }
    ParseRetsReply(eventList);
    ParsingFinished();
}

RetsXmlTextEventPtr CLASS::GetBodyEventFromStandardResponse(
    RetsXmlEventListPtr eventList)
{
    RetsXmlTextEventPtr bodyEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    
    if (eventList->size() == 8)
    {
        startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
        AssertEquals("RETS", startEvent->GetName());
        AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
        startEvent = RetsXmlParser::AssertStartEvent(eventList->at(1));
        AssertEquals("RETS-RESPONSE", startEvent->GetName());
        bodyEvent = RetsXmlParser::AssertTextEvent(eventList->at(2));
        endEvent = RetsXmlParser::AssertEndEvent(eventList->at(3));
        AssertEquals("RETS-RESPONSE", endEvent->GetName());
        startEvent = RetsXmlParser::AssertStartEvent(eventList->at(4));
        AssertEquals("RETS-STATUS", startEvent->GetName());
        AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
        endEvent = RetsXmlParser::AssertEndEvent(eventList->at(5));
        AssertEquals("RETS-STATUS", endEvent->GetName());
        endEvent = RetsXmlParser::AssertEndEvent(eventList->at(6));
        AssertEquals("RETS", endEvent->GetName());
        RetsXmlParser::AssertEndDocumentEvent(eventList->at(7));
    }
    else if (eventList->size() == 6)
    {
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
        RetsXmlParser::AssertEndDocumentEvent(eventList->at(5));
    }
    else if (eventList->size() == 5)
    {
        startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
        AssertEquals("RETS", startEvent->GetName());
        AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
        startEvent = RetsXmlParser::AssertStartEvent(eventList->at(1));
        AssertEquals("RETS-RESPONSE", startEvent->GetName());
        endEvent = RetsXmlParser::AssertEndEvent(eventList->at(2));
        AssertEquals("RETS-RESPONSE", endEvent->GetName());
        endEvent = RetsXmlParser::AssertEndEvent(eventList->at(3));
        AssertEquals("RETS", endEvent->GetName());
        RetsXmlParser::AssertEndDocumentEvent(eventList->at(4));
    }
    else if (eventList->size() == 3)
    {
        startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
        AssertEquals("RETS", startEvent->GetName());
        AssertNotEquals("0", startEvent->GetAttributeValue("ReplyCode"));
        endEvent = RetsXmlParser::AssertEndEvent(eventList->at(1));
        AssertEquals("RETS", endEvent->GetName());
        RetsXmlParser::AssertEndDocumentEvent(eventList->at(2));
    }    
    else
    {
        ostringstream message;
        message << "Unexpected XML event list for standard RETS response: "
                << Output(*eventList);
        throw RetsException(message.str());
    }
        
    return bodyEvent;
}

RetsXmlTextEventPtr CLASS::GetBodyEventFromResponseWithNoRetsResponse(
    RetsXmlEventListPtr eventList)
{
    RetsXmlTextEventPtr bodyEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    
    AssertEventListSize(4, eventList);
    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
    AssertEquals("RETS", startEvent->GetName());
    AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    bodyEvent = RetsXmlParser::AssertTextEvent(eventList->at(1));
    endEvent = RetsXmlParser::AssertEndEvent(eventList->at(2));
    AssertEquals("RETS", endEvent->GetName());
    RetsXmlParser::AssertEndDocumentEvent(eventList->at(3));
    
    return bodyEvent;
}

RetsXmlTextEventPtr CLASS::GetBodyEventFromShortResponse(
    RetsXmlEventListPtr eventList)
{
    RetsXmlTextEventPtr bodyEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    RetsXmlTextEventPtr textEvent;
    
    AssertEventListSize(3, eventList);
    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
    AssertEquals("RETS", startEvent->GetName());
    AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    endEvent = RetsXmlParser::AssertEndEvent(eventList->at(1));
    AssertEquals("RETS", endEvent->GetName());
    RetsXmlParser::AssertEndDocumentEvent(eventList->at(2));
    
    return bodyEvent;
}

void CLASS::ParseBody(string body)
{
    StringVector lines;
    ba::split(lines, body, ba::is_any_of("\n"));
    StringVector::const_iterator line;
    for (line = lines.begin(); line != lines.end(); line++)
    {
        if (line->empty() || (*line).empty())
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

        if (key == "INFO")
        {
            mSessionInfo.ParseInfo(value);
        }
        else
        {
            mValues[key] = value;
        }
    }
}

void CLASS::ParseRetsReply(RetsXmlEventListPtr eventList)
{
    RetsXmlStartElementEventPtr startEvent;

    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
    string name = startEvent->GetName();

    if (name == "RETS")
    {
        istringstream replyCodeString(
                                      startEvent->GetAttributeValue("ReplyCode"));
        int replyCode;
        try
        {
            replyCodeString >> replyCode;
        }
        catch (std::exception &)
        {
            replyCode = 20036; // Miscellaneous Error
        }
        
        if (replyCode != 0)
        {
            mRetsReplyText = startEvent->GetAttributeValue("ReplyText");
            mRetsReplyCode = replyCode;
        }
    }    
}

int CLASS::GetRetsReplyCode() const
{
    return mRetsReplyCode;
}

string CLASS::GetRetsReplyText() const
{
    return mRetsReplyText;
}

string CLASS::GetValue(string key) const
{
    ba::to_upper(key);
    string value;

    if (mRetsVersion >= RETS_1_8)
    {
        value = mSessionInfo.GetValue(key);
    }
    
    if (value.empty())
    {
        StringMap::const_iterator iter = mValues.find(key);
        
        if (iter != mValues.end())
        {
            value = iter->second;
        }
    }
    return value;
}

void CLASS::ParsingFinished()
{
}

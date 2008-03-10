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
#include <iostream>
#include "librets/CurlStream.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlAttribute.h"
#include "librets/RetsXmlEvent.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/RetsXmlEndDocumentEvent.h"
#include "librets/RetsException.h"
#include "librets/RetsHttpClient.h"

using namespace librets;
using std::string;
using std::stringstream;
using std::ostringstream;
using std::istream;
namespace b = boost;

ExpatXmlParser::ExpatXmlParser(string inputString, const char *encoding)
                            : mHttpClient()
{
    istreamPtr inputStream(new stringstream(inputString));
    init(inputStream, encoding);
}

ExpatXmlParser::ExpatXmlParser(istreamPtr inputStream, const char *encoding)
                            : mHttpClient()
{
    init(inputStream, encoding);
}

ExpatXmlParser::ExpatXmlParser(RetsHttpClientPtr httpClient, 
                                istreamPtr inputStream, 
                                const char *encoding)
{
    init(inputStream, encoding);
    mHttpClient = httpClient;
}

void ExpatXmlParser::init(istreamPtr inputStream, const char *encoding)
{
    mParser = XML_ParserCreate((XML_Char*) encoding);
    if (!mParser)
    {
        throw RetsException("Could not allocate XML parser");
    }
    
    XML_SetUserData(mParser, this);
    XML_SetStartElementHandler(mParser, ExpatXmlParser::StartElement);
    XML_SetEndElementHandler(mParser, ExpatXmlParser::EndElement);
    XML_SetCharacterDataHandler(mParser, ExpatXmlParser::CharacterData);

    mIsDone = false;
    mInputStream = inputStream;
}

ExpatXmlParser::~ExpatXmlParser()
{
    XML_ParserFree(mParser);
}

bool ExpatXmlParser::HasNext()
{
    return (!mIsDone);
}

RetsXmlEventPtr ExpatXmlParser::GetNextEvent()
{
    RetsXmlEventPtr event = GetNextEventWithoutCoalescing();
    if (event->GetType() == RetsXmlEvent::TEXT)
    {
        RetsXmlTextEventPtr textEvent =
            b::dynamic_pointer_cast<RetsXmlTextEvent>(event);
        CoalesceTextEvents(textEvent);
    }

    if (event->GetType() == RetsXmlEvent::END_DOCUMENT)
    {
        mIsDone = true;
    }

    return event;
}

void ExpatXmlParser::CoalesceTextEvents(RetsXmlTextEventPtr textEvent)
{
    while (true)
    {
        RetsXmlEventPtr event = GetNextEventWithoutCoalescing();
        if (event->GetType() != RetsXmlEvent::TEXT)
        {
            // Put it back
            mEvents.push_front(event);
            break;
        }
        else
        {
            RetsXmlTextEventPtr nextTextEvent =
                b::dynamic_pointer_cast<RetsXmlTextEvent>(event);
            textEvent->AppendText(nextTextEvent->GetText());
        }
    }
}

RetsXmlEventPtr ExpatXmlParser::GetNextEventWithoutCoalescing()
{
    bool atEof = false;
    bool isCurlStream = (typeid(*mInputStream) == typeid(CurlStream));
    bool isLast = false;
    istreamPtr inputStream = b::dynamic_pointer_cast<CurlStream>(mInputStream);

    if (!inputStream)
    {
        inputStream = mInputStream;
    }
    
    if (mIsDone)
    {
        throw RetsException("XML parser is finished");
    }

    while (mEvents.empty())
    {
        if (!inputStream->eof())
        {
            char buf[512];

            inputStream->read(buf, sizeof(buf));
            int len = inputStream->gcount();
            isLast = inputStream->eof();
                
            if (XML_Parse(mParser, buf, len, isLast) == XML_STATUS_ERROR)
            {
                int lineNumber = GetCurrentLineNumber();
                int columnNumber = GetCurrentColumnNumber();
                string errorString =
                    XML_ErrorString(XML_GetErrorCode(mParser));
                ostringstream message;
                message << "XML parse error at " << lineNumber << ":"
                        << columnNumber << ": " + errorString;
                throw RetsException(message.str());
            }
            if (isLast)
            {
                RetsXmlEndDocumentEventPtr endDocument(
                    new RetsXmlEndDocumentEvent());
                mEvents.push_back(endDocument);
            }
        }
        else
        {
            throw RetsException("Parsing past end of XML document");
        }
    }

    RetsXmlEventPtr event = mEvents.front();
    mEvents.pop_front();
    return event;
}

int ExpatXmlParser::GetCurrentLineNumber() const
{
    return XML_GetCurrentLineNumber(mParser);
}

int ExpatXmlParser::GetCurrentColumnNumber() const
{
    return XML_GetCurrentColumnNumber(mParser);
}

void ExpatXmlParser::StartElement(void * userData, const char * name,
                                  const char **atts)
{
    ExpatXmlParser * parser = (ExpatXmlParser *) userData;
    int lineNumber = parser->GetCurrentLineNumber();
    int columnNumber = parser->GetCurrentColumnNumber();
    RetsXmlStartElementEventPtr event(
        new RetsXmlStartElementEvent(lineNumber, columnNumber));
    event->SetName(name);
    for (int i = 0; atts[i] != 0; i += 2)
    {
        RetsXmlAttributePtr attribute(
            new RetsXmlAttribute(atts[i], atts[i+1]));
        event->AddAttribute(attribute);
    }
    parser->mEvents.push_back(event);
#if 0
    std::cout << event << std::endl;
#endif
}

void ExpatXmlParser::EndElement(void * userData, const char * name)
{
    ExpatXmlParser * parser = (ExpatXmlParser *) userData;
    int lineNumber = parser->GetCurrentLineNumber();
    int columnNumber = parser->GetCurrentColumnNumber();
    RetsXmlEndElementEventPtr event(
        new RetsXmlEndElementEvent(lineNumber, columnNumber));
    event->SetName(name);
    parser->mEvents.push_back(event);
#if 0
    std::cout << event << std::endl;
#endif
}

void ExpatXmlParser::CharacterData(void * userData, const XML_Char * s,
                                   int len)
{
    string text((const char *) s, len);
    ExpatXmlParser * parser = (ExpatXmlParser *) userData;
    int lineNumber = parser->GetCurrentLineNumber();
    int columnNumber = parser->GetCurrentColumnNumber();
    RetsXmlTextEventPtr event(
        new RetsXmlTextEvent(lineNumber, columnNumber));
    event->AppendText(text);
    parser->mEvents.push_back(event);
#if 0
    std::cout << event << std::endl;
#endif
}

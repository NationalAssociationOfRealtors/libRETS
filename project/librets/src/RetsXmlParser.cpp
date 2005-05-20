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

RetsXmlStartElementEventPtr CLASS::AssertNextIsStartEvent()
{
    RetsXmlEventPtr event = GetNextSkippingEmptyText();
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

RetsXmlEndElementEventPtr CLASS::AssertNextIsEndEvent()
{
    RetsXmlEventPtr event = GetNextSkippingEmptyText();
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

RetsXmlTextEventPtr CLASS::AssertNextIsTextEvent()
{
    RetsXmlEventPtr event = GetNextSkippingEmptyText();
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


#include <sstream>
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
    startEvent = assertNextIsStart(xmlParser);
    assertEquals("RETS-RESPONSE", startEvent->GetName());
    textEvent = assertNextIsText(xmlParser);
    body = textEvent->GetText();
    endEvent = assertNextIsEnd(xmlParser);
    assertEquals("RETS-RESPONSE", endEvent->GetName());
    endEvent = assertNextIsEnd(xmlParser);
    assertEquals("RETS", endEvent->GetName());
    
    StringVectorPtr lines;
    lines = split_no_empties(body, "\n");
    StringVector::const_iterator line;
    for (line = lines->begin(); line != lines->end(); line++)
    {
        StringVectorPtr keyValue;
        keyValue = split(*line, "=");
        if (keyValue->size() != 2)
        {
            throw RetsException("Invalid key value pair" + *line);
        }
        string key = toUpper(trim((*keyValue)[0]));
        string value = trim((*keyValue)[1]);
        mValues[key] = value;
    }
    ParsingFinished();
}

string CLASS::GetValue(string name) const
{
    string key = toUpper(name);
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

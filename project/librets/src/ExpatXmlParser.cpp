#include <sstream>
#include <iostream>
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlAttribute.h"
#include "librets/RetsXmlEvent.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/RetsException.h"

using namespace librets;
using std::string;
using std::stringstream;
using std::ostringstream;
namespace b = boost;

ExpatXmlParser::ExpatXmlParser(string inputString)
{
    istreamPtr inputStream(new stringstream(inputString));
    init(inputStream);
}

ExpatXmlParser::ExpatXmlParser(istreamPtr inputStream)
{
    init(inputStream);
}

void ExpatXmlParser::init(istreamPtr inputStream)
{
    mParser = XML_ParserCreate(NULL);
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

    if (mEvents.empty() && mInputStream->eof())
    {
        mIsDone = true;
    }

    return event;
}

void ExpatXmlParser::CoalesceTextEvents(RetsXmlTextEventPtr textEvent)
{
    bool done = mIsDone;
    while (!done)
    {
        RetsXmlEventPtr event = GetNextEventWithoutCoalescing();
        if (event->GetType() != RetsXmlEvent::TEXT)
        {
            // Put it back
            mEvents.push_front(event);
            done = true;
        }
        else
        {
            RetsXmlTextEventPtr textEvent2 =
                b::dynamic_pointer_cast<RetsXmlTextEvent>(event);
            textEvent->AppendText(textEvent2->GetText());
            done = mIsDone;
        }
    }
}

RetsXmlEventPtr ExpatXmlParser::GetNextEventWithoutCoalescing()
{
    if (mIsDone)
    {
        throw RetsException("XML parser is finished");
    }

    while (mEvents.empty())
    {
        if (!mInputStream->eof())
        {
            char buf[512];
            mInputStream->read(buf, sizeof(buf));
            int len = mInputStream->gcount();
            if (XML_Parse(mParser, buf, len, false) == XML_STATUS_ERROR)
            {
                int lineNumber = XML_GetCurrentLineNumber(mParser);
                int columnNumber = XML_GetCurrentColumnNumber(mParser);
                string errorString =
                    XML_ErrorString(XML_GetErrorCode(mParser));
                ostringstream message;
                message << "XML parse error at " << lineNumber << ":"
                        << columnNumber << ": " + errorString;
                throw RetsException(message.str());
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

void ExpatXmlParser::StartElement(void * userData, const char * name,
                                  const char **atts)
{
    ExpatXmlParser * parser = (ExpatXmlParser *) userData;
    RetsXmlStartElementEventPtr event(new RetsXmlStartElementEvent());
    event->SetName(name);
    for (int i = 0; atts[i] != 0; i += 2)
    {
        RetsXmlAttributePtr attribute(
            new RetsXmlAttribute(atts[i], atts[i+1]));
        event->AddAttribute(attribute);
    }
    parser->mEvents.push_back(event);
}

void ExpatXmlParser::EndElement(void * userData, const char * name)
{
    ExpatXmlParser * parser = (ExpatXmlParser *) userData;
    RetsXmlEndElementEventPtr event(new RetsXmlEndElementEvent());
    event->SetName(name);
    parser->mEvents.push_back(event);
}

void ExpatXmlParser::CharacterData(void * userData, const XML_Char * s,
                                   int len)
{
    string text((const char *) s, len);
    ExpatXmlParser * parser = (ExpatXmlParser *) userData;
    RetsXmlTextEventPtr event(new RetsXmlTextEvent());
    event->AppendText(text);
    parser->mEvents.push_back(event);
}

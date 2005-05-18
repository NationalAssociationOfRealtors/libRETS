#include <sstream>
#include "librets/XmlMetadataParser.h"
#include "librets/XmlMetadataElementFactory.h"
#include "librets/DefaultMetadataElementFactory.h"
#include "librets/MetadataElementCollector.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/MetadataElement.h"
#include "librets/RetsException.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostringstream;
namespace b = boost;

#define CLASS XmlMetadataParser

CLASS::CLASS(MetadataElementCollectorPtr elementCollector)
{
    mElementCollector = elementCollector;
    mElementFactory.reset(new DefaultMetadataElementFactory());
}

void CLASS::SetElementFactory(XmlMetadataElementFactoryPtr elementFactory)
{
    mElementFactory = elementFactory;
}

void CLASS::Parse(istreamPtr inputStream)
{
    mXmlParser.reset(new ExpatXmlParser(inputStream));
    RetsXmlStartElementEventPtr metadataEvent;
    while (mXmlParser->HasNext())
    {
        RetsXmlEventPtr event = mXmlParser->GetNextSkippingEmptyText();
        RetsXmlStartElementEventPtr metadataEvent
            = b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
        if (!metadataEvent)
        {
            continue;
        }
        
        string name = metadataEvent->GetName();
        if (name == "METADATA-SYSTEM")
        {
            HandleSystemMetadata(metadataEvent);
        }
        else if (startsWith(name, "METADATA-"))
        {
            HandleOtherMetadata(metadataEvent);
        }
    }
}

void CLASS::HandleSystemMetadata(RetsXmlStartElementEventPtr metadataEvent)
{
    MetadataElementPtr element;
    while (mXmlParser->HasNext())
    {
        RetsXmlEventPtr event = mXmlParser->GetNextSkippingEmptyText();
        RetsXmlStartElementEventPtr startEvent =
            b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
        RetsXmlEndElementEventPtr endEvent =
            b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
        if (startEvent)
        {
            if (startEvent->GetName() == "SYSTEM")
            {
                element =
                    mElementFactory->CreateMetadataElement(metadataEvent);
                string value = startEvent->GetAttributeValue("SystemID");
                element->SetAttribute("SystemID", value);
                value = startEvent->GetAttributeValue("SystemDescription");
                element->SetAttribute("SystemDescription", value);
                mXmlParser->AssertNextIsEndEvent();
            }
            else if (startEvent->GetName() == "COMMENTS")
            {
                RetsXmlTextEventPtr textEvent =
                    mXmlParser->AssertNextIsTextEvent();
                element->SetAttribute("COMMENTS", textEvent->GetText());
                mXmlParser->AssertNextIsEndEvent();
            }
        }
        else if (endEvent && endEvent->GetName() == "METADATA-SYSTEM")
        {
            mElementCollector->AddElement(element);
            break;
        }
    }
}

void CLASS::HandleOtherMetadata(RetsXmlStartElementEventPtr metadataEvent)
{
    StringVectorPtr columns;
    while (mXmlParser->HasNext())
    {
        RetsXmlEventPtr event = mXmlParser->GetNextSkippingEmptyText();
        RetsXmlStartElementEventPtr startEvent =
            b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
        RetsXmlEndElementEventPtr endEvent =
            b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
        if (startEvent)
        {
            if (startEvent->GetName() == "COLUMNS")
            {
                RetsXmlTextEventPtr textEvent =
                    mXmlParser->AssertNextIsTextEvent();
                columns = split(textEvent->GetText(), "\t");
                if (columns->size() < 2)
                {
                    ostringstream message;
                    message << "Unknown column format: " << Output(*columns);
                    throw RetsException(message.str());
                }
            }
            else if (startEvent->GetName() == "DATA")
            {
                RetsXmlTextEventPtr textEvent =
                    mXmlParser->AssertNextIsTextEvent();
                StringVectorPtr data = split(textEvent->GetText(), "\t");
                if (!columns)
                {
                    throw RetsException("Got data withouht columns");
                }
                if (data->size() < columns->size())
                {
                    ostringstream message;
                    message << "Unknown data format: " << Output(*data);
                    throw RetsException(message.str());
                }
                MetadataElementPtr element =
                    mElementFactory->CreateMetadataElement(metadataEvent);
                for (StringVector::size_type index = 1;
		     index < data->size(); index++)
                {
                    string name = columns->at(index);
                    string value = data->at(index);
                    element->SetAttribute(name, value);
                }
                mElementCollector->AddElement(element);
            }
        }
        else if (endEvent &&
                 startsWith(endEvent->GetName(), "METADATA-"))
        {
            break;
        }
    }
}

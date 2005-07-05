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
namespace ba = boost::algorithm;

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
        else if (ba::starts_with(name, "METADATA-"))
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
                mXmlParser->AssertNextIsEndEvent("SYSTEM: ");
            }
            else if (startEvent->GetName() == "COMMENTS")
            {
                RetsXmlTextEventPtr textEvent =
                    mXmlParser->AssertNextIsTextEvent("COMMENTS: ");
                element->SetAttribute("COMMENTS", textEvent->GetText());
                mXmlParser->AssertNextIsEndEvent("COMMENTS: ");
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
                HandleColumns();
            }
            else if (startEvent->GetName() == "DATA")
            {
                HandleData(metadataEvent);
            }
        }
        else if (endEvent &&
                 ba::starts_with(endEvent->GetName(), "METADATA-"))
        {
            break;
        }
    }
}

void CLASS::HandleColumns()
{
    RetsXmlTextEventPtr textEvent =
    mXmlParser->AssertNextIsTextEvent("COLUMNS: ");
    string text = textEvent->GetText();
    ba::split(columns, text, ba::is_any_of("\t"));
    if (columns.size() < 2)
    {
        ostringstream message;
        message << "Unknown column format: " << Output(columns);
        throw RetsException(message.str());
    }
}

void CLASS::HandleData(RetsXmlStartElementEventPtr metadataEvent)
{
    RetsXmlEventPtr event = mXmlParser->GetNextSkippingEmptyText();
    RetsXmlEndElementEventPtr endEvent =
        b::dynamic_pointer_cast<RetsXmlEndElementEvent>(event);
    if (endEvent && (endEvent->GetName() == "DATA"))
    {
        // Skip a blank data row (or a row with only whitespace)
        return;
    }

    RetsXmlTextEventPtr textEvent = mXmlParser->AssertTextEvent(event,
                                                                "DATA: ");
    string text = textEvent->GetText();
    StringVector data;
    ba::split(data, text, ba::is_any_of("\t"));
    if (columns.empty())
    {
        throw RetsException("Got data without columns");
    }
    if (data.size() < columns.size())
    {
        ostringstream message;
        message << "Unknown data format: " << Output(data);
        throw RetsException(message.str());
    }
    MetadataElementPtr element =
        mElementFactory->CreateMetadataElement(metadataEvent);
    for (StringVector::size_type index = 1;
         index < columns.size(); index++)
    {
        string name = columns.at(index);
        string value = data.at(index);
        element->SetAttribute(name, value);
    }
    mElementCollector->AddElement(element);
}

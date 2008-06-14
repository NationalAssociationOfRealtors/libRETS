/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
#include <boost/version.hpp>
#include <stdexcept>
#include "librets/CurlStream.h"
#include "librets/ServerInfoResponse.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/RetsException.h"
#include "librets/RetsReplyException.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::vector;
using std::istringstream;
using std::ostringstream;
using std::invalid_argument;
namespace b = boost;
namespace ba = boost::algorithm;

ServerInformationResponse::ServerInformationResponse() 
            : mParseInputStream()
{
    mEncoding = RETS_XML_DEFAULT_ENCODING;
    mReplyCode = 0;
    mReplyText.clear();
    mValues.clear();
    mClasses.clear();
    mResources.clear();
    mParameters.clear();
}

ServerInformationResponse::~ServerInformationResponse()
{
}

void ServerInformationResponse::FixCompactArray(StringVector & compactArray,
                                      std::string context)
{
    if (compactArray.size() < 2)
    {
        ostringstream message;
        message << "Unknown compact format: " << context << ": "
                << Output(compactArray);
        throw RetsException(message.str());
    }

    if (!compactArray.front().empty())
    {
        ostringstream message;
        message << "First element should be empty: " << context << ": "
                << Output(compactArray);
        throw RetsException(message.str());
    }
    compactArray.erase(compactArray.begin());
    
#if BOOST_VERSION >= 103300
    if (!compactArray.back().empty())
    {
        ostringstream message;
        message << "Last element should be empty: " << context << ": "
                << Output(compactArray);
        throw RetsException(message.str());
    }
    compactArray.pop_back();
#endif
}

void ServerInformationResponse::Parse(istreamPtr inputStream)
{
    SetInputStream(inputStream);
    
    while (mXmlParser->HasNext())
    {
        RetsXmlEventPtr event = mXmlParser->GetNextSkippingEmptyText();
        RetsXmlStartElementEventPtr startEvent
            = b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
        if (!startEvent)
        
        {
            continue;
        }
        
        string name = startEvent->GetName();
        if (name == "RETS")
        {
            istringstream replyCodeString(
                startEvent->GetAttributeValue("ReplyCode"));
            int replyCode;
            replyCodeString >> replyCode;
            if (replyCode != 0)
            {
                string meaning = startEvent->GetAttributeValue("ReplyText");
                string extendedMeaning;
                /*
                 * There can be an optional text event as the next event, so we
                 * try this in a try/catch block.
                 */
                try
                {
                    RetsXmlTextEventPtr textEvent =
                        mXmlParser->AssertNextIsTextEvent();
                    extendedMeaning = textEvent->GetText();
                }
                catch (RetsException e)
                {
                    throw RetsReplyException(replyCode, meaning);
                }
                /*
                 * If we're here, we have an extended message. It could contain one
                 * or more newlines, so remove them.
                 */
                ba::erase_all(extendedMeaning, "\n");
                throw RetsReplyException(replyCode, meaning, extendedMeaning);
            }
            mReplyCode = 0;
            mReplyText.clear();
        }
        else if (name == "ServerInformation")
        {
            int i;
            RetsXmlEventListPtr eventList = mXmlParser->GetEventListSkippingEmptyText();
            
            int events = eventList->size();
     
            for (i = 0; i < events; i+=3)
            {
                RetsXmlEventPtr serverEvent = eventList->at(i);
            
                RetsXmlStartElementEventPtr startEvent = b::dynamic_pointer_cast<RetsXmlStartElementEvent>(serverEvent);
                if (!startEvent)
                {
                    break;
                }
                /*
                 * We've found the <ServerInformation> tag. Parse it.
                 */
                string parameterName = startEvent->GetAttributeValue("name");
                string resourceName = startEvent->GetAttributeValue("resource");
                string className = startEvent->GetAttributeValue("class");
                RetsXmlTextEventPtr textEvent = RetsXmlParser::AssertTextEvent(eventList->at(i + 1));
                RetsXmlEndElementEventPtr endEvent = RetsXmlParser::AssertEndEvent(eventList->at(i + 2));
                
                /*
                 * Save the values.
                 */
                mParameters.push_back(parameterName);
                mResources[parameterName] = resourceName;
                mClasses[parameterName] = className;
                string value = textEvent->GetText();
                ba::trim(value);
                mValues[parameterName] = value;
            }

            // </ServerInformation>
            RetsXmlParser::AssertEndEvent(eventList->at(i));
            // </RETS>
            RetsXmlParser::AssertEndEvent(eventList->at(i + 1));
            // End of Document.
            RetsXmlParser::AssertEndDocumentEvent(eventList->at(i + 2));
         }
        else if (name == "RETS-STATUS")
        {
            istringstream replyCodeString(
                startEvent->GetAttributeValue("ReplyCode"));
            replyCodeString >> mReplyCode;
            mReplyText.clear();
            if (mReplyCode != 0)
            {
                mReplyText = startEvent->GetAttributeValue("ReplyText");
            }
        }
    }
}

const StringVector ServerInformationResponse::GetParameters()
{
    return mParameters;
}

string ServerInformationResponse::GetClass(string parameterName)
{
    StringMap::const_iterator value = mClasses.find(parameterName);
    
    if (value == mClasses.end())
    {
        return "";
    }
    
    return value->second;
}

string ServerInformationResponse::GetResource(string parameterName)
{
    StringMap::const_iterator value = mResources.find(parameterName);
    
    if (value == mResources.end())
    {
        return "";
    }
    
    return value->second;
}

string ServerInformationResponse::GetValue(string parameterName)
{
    StringMap::const_iterator value = mValues.find(parameterName);
    
    if (value == mValues.end())
    {
        return "";
    }
    
    return value->second;
}

void ServerInformationResponse::SetEncoding(EncodingType encoding)
{
    mEncoding = encoding;
}

EncodingType ServerInformationResponse::GetEncoding()
{
    return mEncoding;
}

int ServerInformationResponse::GetReplyCode()
{
    return mReplyCode;
}

string ServerInformationResponse::GetReplyText()
{
    return mReplyText;
}

void ServerInformationResponse::SetInputStream(istreamPtr inputStream)
{
    /*
     * Initialize for streaming mode.
     */
    mParseInputStream =  inputStream;
    
    ExpatXmlParserPtr XmlParser( new ExpatXmlParser(inputStream,
                    (mEncoding == RETS_XML_ISO_ENCODING 
                        ? "iso-8859-1"    
                        : "US-ASCII")));
    mXmlParser = XmlParser;
}


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
#include <boost/lexical_cast.hpp>                                               
#include "librets/LogoutResponse.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"

using namespace librets;
using std::string;
using std::istringstream;
using boost::lexical_cast;
using boost::bad_lexical_cast;

#define CLASS LogoutResponse

void CLASS::ParsingFinished()
{
    try
    {
        mConnectTime = lexical_cast<int>(GetValue("ConnectTime"));
    }
    catch(bad_lexical_cast &)
    {
        mConnectTime = 0;
    }
}

string CLASS::GetBillingInfo() const
{
    return GetValue("Billing");
}

string CLASS::GetLogoutMessage() const
{
    return GetValue("SignOffMessage");
}

int CLASS::GetConnectTime() const
{
    return mConnectTime;
}

RetsXmlTextEventPtr CLASS::GetBodyEventFromEmptyLogoutResponse(
    RetsXmlEventListPtr eventList)
{
    RetsXmlTextEventPtr bodyEvent;
    RetsXmlStartElementEventPtr startEvent;
    RetsXmlEndElementEventPtr endEvent;
    
    AssertEventListSize(4, eventList);
    startEvent = RetsXmlParser::AssertStartEvent(eventList->at(0));
    AssertEquals("RETS", startEvent->GetName());
    AssertEquals("0", startEvent->GetAttributeValue("ReplyCode"));
    RetsXmlParser::AssertTextEvent(eventList->at(1));
    endEvent = RetsXmlParser::AssertEndEvent(eventList->at(2));
    AssertEquals("RETS", endEvent->GetName());
    RetsXmlParser::AssertEndDocumentEvent(eventList->at(3));
    
    return bodyEvent;
}

RetsXmlTextEventPtr CLASS::GetBodyEvent(RetsXmlEventListPtr eventList,
                                        RetsVersion retsVersion)
{
    RetsXmlTextEventPtr bodyEvent;
    if (eventList->size() == 4)
    {
        bodyEvent = GetBodyEventFromEmptyLogoutResponse(eventList);
    }
    else if (eventList->size() == 3)
    {
        bodyEvent = GetBodyEventFromShortResponse(eventList);
    }
    else
    {
        bodyEvent = GetBodyEventFromStandardResponse(eventList);
    }
    return bodyEvent;
}

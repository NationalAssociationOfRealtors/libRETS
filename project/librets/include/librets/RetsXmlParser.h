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

#ifndef LIBRETS_RETS_XML_PARSER_H
#define LIBRETS_RETS_XML_PARSER_H
/** 
 * @file RetsXmlParser.h
 * (Internal) Contains the primary XML Parser interface class for use with libexpat.
 */
/// @cond MAINTAINER

#include "librets/std_forward.h"
#include "librets/xml_forward.h"

namespace librets {
/**
 * (Internal) RetsXmlParser is the primary XML Parser class for use with libexpat.
 */
class RetsXmlParser
{
  public:
    /**
     * Create a default XML parser around the speicified input stream.
     * @param inputStream A pointer to the input stream to be parsed.
     * @returns A pointer to the parser.
     */
    static RetsXmlParserPtr CreateDefault(istreamPtr inputStream);

    virtual ~RetsXmlParser();

    /**
     * Indicates whether or not there are additional element to be parsed in
     * this data stream.
     * @return TRUE if there is more to be parsed.
     */
    virtual bool HasNext() = 0;

    /**
     * Returns next XML event, blocking if necessary.
     *
     * @return Next XML event
     * @throw RetsException
     */
    virtual RetsXmlEventPtr GetNextEvent() = 0;
    
    /**
     * Returns all remaining events as a vector.
     *
     * @return Vector of events.
     */
    RetsXmlEventListPtr GetEventList();

    RetsXmlEventPtr GetNextSkippingEmptyText();
    
    /**
     * Returns all remaining events as a vector, ingoring all empty
     * text events.
     *
     * @return Vector of events.
     */
    RetsXmlEventListPtr GetEventListSkippingEmptyText();

    /**
     * Assert that the next event is a start element event.
     * A side effect of this call is that the next element in the input stream
     * is parsed.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the next event.
     * @throws RetsException if the next element is not a start event.
     */
    RetsXmlStartElementEventPtr AssertNextIsStartEvent(std::string prefix = "");

    /**
     * Assert that the event is a start element event.
     * @param event A pointer to the object containing the event.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the event.
     * @throws RetsException if the element is not a start event.
     */
    static RetsXmlStartElementEventPtr
        AssertStartEvent(RetsXmlEventPtr event, std::string prefix = "");

    /**
     * Assert that the next event is an end element event.
     * A side effect of this call is that the next element in the input stream
     * is parsed.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the next event.
     * @throws RetsException if the next element is not an end event.
     */
    RetsXmlEndElementEventPtr AssertNextIsEndEvent(std::string prefix = "");

    /**
     * Assert that the event is an end element event.
     * @param event A pointer to the object containing the event.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the event.
     * @throws RetsException If the next element is not an end event.
     */
    static RetsXmlEndElementEventPtr
        AssertEndEvent(RetsXmlEventPtr event, std::string prefix = "");

    /**
     * Assert that the next event is a text element event.
     * A side effect of this call is that the next element in the input stream
     * is parsed.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the next event.
     * @throws RetsException if the next element is not a text event.
     */
    RetsXmlTextEventPtr AssertNextIsTextEvent(std::string prefix = "");

    /**
     * Assert that the event is a text element event.
     * @param event A pointer to the object containing the event.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the event.
     * @throws RetsException If the element is not a text event.
     */
    static RetsXmlTextEventPtr
        AssertTextEvent(RetsXmlEventPtr event, std::string prefix = "");

    /**
     * Assert that the next event is the end document event.
     * A side effect of this call is that the next element in the input stream
     * is parsed.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the next event.
     * @throws RetsException if the next element is not the end document event.
     */
    RetsXmlEndDocumentEventPtr
        AssertNextIsEndDocumentEvent(std::string prefix = "");

    /**
     * Assert that the event is the end document event.
     * @param event A pointer to the object containing the event.
     * @param prefix A string to be included when reporting errors.
     * @returns A RetsXmlStartElementEvent pointer to the event.
     * @throws RetsException If the element is not the end document event.
     */
    static RetsXmlEndDocumentEventPtr
        AssertEndDocumentEvent(RetsXmlEventPtr event, std::string prefix = "");
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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
#ifndef LIBRETS_RETS_XML_END_ELEMENT_EVENT_H
#define LIBRETS_RETS_XML_END_ELEMENT_EVENT_H
/** 
 * @file RetsXmlEndElementEvent.h
 * (Internal) Contains the XML Parser End Event interface class for use with libexpat.
 */
/// @cond MAINTAINER

#include "librets/RetsXmlEvent.h"

namespace librets {
/**
 * (Internal) RetsXmlEndElementEvent is a class that handles the Expat "end" element 
 * XML event.
 */
class RetsXmlEndElementEvent : public RetsXmlEvent
{
  public:
    /**
     * Contstruct the object with a default line and column number. These numbers
     * should reflect the line/column from the XML stream where this element can
     * be found and is used for debugging.
     */
    RetsXmlEndElementEvent(int lineNumber = -1, int columnNumber = -1);
    
    virtual ~RetsXmlEndElementEvent();

    /**
     * Always returns END_ELEMENT.
     * @return END_ELEMENT
     */
    virtual Type GetType() const;
    /**
     * Sets the name of this event.
     * @param name A string containing the name of the XML element being parsed.
     */
    void SetName(std::string name);
    /**
     * Returns the name of the attribute.
     * @return string containing the name of the attribute.
     */
    std::string GetName() const;
    /**
     * Prints the attribute in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;
    /**
     * Checks to see if the attribute names are identical between
     * two RetsXmlEndElementEvent objects.
     * @return TRUE if the name and value for both attributes matches.
     */
    virtual bool Equals(const RetsObject * rhs) const;

  private:
    std::string mName;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

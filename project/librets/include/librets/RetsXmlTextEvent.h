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
#ifndef LIBRETS_RETS_XML_TEXT_EVENT_H
#define LIBRETS_RETS_XML_TEXT_EVENT_H
/** 
 * @file RetsXmlTextEvent.h
 * (Internal) Contains the XML Parser Text Event interface class for use with libexpatL.
 */
/// @cond MAINTAINER

#include <sstream>
#include "librets/RetsXmlEvent.h"

namespace librets {
/**
 * (Internal) RetsXmlTextEvent is a class that handles the Expat "end" element 
 * XML event.
 */
class RetsXmlTextEvent : public RetsXmlEvent
{
  public:
    /**
     * Contstruct the object with a default line and column number. These numbers
     * should reflect the line/column from the XML stream where this element can
     * be found and is used for debugging.
     */
    RetsXmlTextEvent(int lineNumber = -1, int columnNumber = -1);
    
    virtual ~RetsXmlTextEvent();

    /**
     * Always returns TEXT.
     * @return TEXT
     */
    virtual Type GetType() const;

    /**
     * Append the text.
     * @param text A string containing the text to add.
     */
    void AppendText(std::string text);

    /**
     * Return the current text.
     * @return A string containing the text.
     */
    std::string GetText() const;

    /**
     * Prints the object in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;

    /**
     * Checks to see if the text is identical between two RetsXmlTextEvent objects.
     * @return TRUE if the text matches.
     */
    virtual bool Equals(const RetsObject * rhs) const;

  private:
    std::ostringstream mText;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

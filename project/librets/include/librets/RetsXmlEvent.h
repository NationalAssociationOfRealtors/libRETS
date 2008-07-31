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
#ifndef LIBRETS_RETS_XML_EVENT_H
#define LIBRETS_RETS_XML_EVENT_H
/** 
 * @file RetsXmlEvent.h
 * (Internal) Contains the XML Parser Event interface class for use with libexpat.
 */
/// @cond MAINTAINER

#include <string>
#include "librets/RetsObject.h"

namespace librets {

/**
 * (Internal) RetsXmlEvent is the super class from which the other Xml parsing classes inherit.
 */
class RetsXmlEvent : public RetsObject
{
  public:
    /**
     * Clasify the type of the Xml event.
     */
    enum Type {
        START_ELEMENT,  /**< Indicates this is a start element (&lt;RETS&gt;) */
        END_ELEMENT,    /**< Indicates this is an end element (&lt;/RETS&gt;) */
        TEXT,           /**< Indicates this is a text element */
        END_DOCUMENT,   /**< Indicates this is the end of the XML document */
    };
    /**
     * Default constructor with default line and column numbers.
     */
    RetsXmlEvent();
    /**
     * Constructor specifying the line and column numbers for the element
     * being parsed.
     * @param lineNumber An integer containing the line number
     * @param columnNumber An integer containing the column number
     */
    RetsXmlEvent(int lineNumber, int columnNumber);
    
    virtual ~RetsXmlEvent();

    /**
     * Each chass inheriting from this class must provide this method.
     */
    virtual Type GetType() const = 0;
    
    /**
     * Get the line number for this element.
     * @return An integer containing the line number.
     */
    int GetLineNumber() const;
    
    /**
     * Get the column number for this element.
     * @return An integer containing the column number.
     */
    int GetColumnNumber() const;
    
  protected:
    std::ostream & PrintLineAndColumn(std::ostream & out) const;
    
    int mLineNumber;
    int mColumnNumber;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

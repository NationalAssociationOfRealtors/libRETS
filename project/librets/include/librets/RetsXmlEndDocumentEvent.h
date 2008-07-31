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

#ifndef LIBRETS_RETS_XML_END_DOCUMENT_EVENT_H
#define LIBRETS_RETS_XML_END_DOCUMENT_EVENT_H
/** 
 * @file RetsXmlEndDocumentEvent.h
 * (Internal) Contains the XML End Document Event interface class for use with libexpat.
 */
/// @cond MAINTAINER


#include "librets/RetsXmlEvent.h"

namespace librets {
/**
 * (Internal) RetsXmlEndDocumentEvent is a class that handles the Expat "end document" 
 * XML event.
 * Refer to the libexpat documentation.
 */    
class RetsXmlEndDocumentEvent : public RetsXmlEvent
{
  public:
    /**
     * Contstruct the object with a default line and column number. These numbers
     * should reflect the line/column from the XML stream where this element can
     * be found and is used for debugging.
     */
    RetsXmlEndDocumentEvent(int lineNumber = -1, int columnNumber = -1);
    /**
     * Always returns END_DOCUMENT.
     * @return END_DOCUMENT
     */
    virtual Type GetType() const;
    /**
     * Prints the attribute and value in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;
    /**
     * Checks to see if the attribute names are identical between
     * two RetsXmlEndDocumentEvent objects.
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

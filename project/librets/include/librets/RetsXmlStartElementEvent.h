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
#ifndef LIBRETS_RETS_XML_START_ELEMENT_EVENT_H
#define LIBRETS_RETS_XML_START_ELEMENT_EVENT_H
/** 
 * @file RetsXmlStartElementEvent.h
 * (Internal) Contains the XML Parser Start Element Event interface class for use with libexpatL.
 */
/// @cond MAINTAINER


#include "librets/RetsXmlEvent.h"
#include "librets/xml_forward.h"
#include <ostream>
#include <vector>
#include <map>

namespace librets {

typedef std::vector<RetsXmlAttributePtr> RetsXmlAttributeList;
typedef boost::shared_ptr<RetsXmlAttributeList> RetsXmlAttributeListPtr;

/**
 * (Internal) RetsXmlStartElementEvent is a class that handles the Expat "start" element 
 * XML event.
 */
class RetsXmlStartElementEvent : public RetsXmlEvent
{
  public:
    /**
     * Contstruct the object with a default line and column number. These numbers
     * should reflect the line/column from the XML stream where this element can
     * be found and is used for debugging.
     */
    RetsXmlStartElementEvent(int lineNumber = -1, int columnNumber = -1);
    
    virtual ~RetsXmlStartElementEvent();

    /**
     * Always returns START_ELEMENT.
     * @return START_ELEMENT
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
     * Add the given RetsXmlAttribute object to the vector of attribute values.
     * @param attribute The attribute being pushed to the end of the vector.
     */
    void AddAttribute(RetsXmlAttributePtr attribute);

    /**
     * Add the attribute and value to the vector of attribute values.
     * @param name A string containing the name of the attribute.
     * @param value A string containing the value of the attribute.
     */
    void AddAttribute(std::string name, std::string value);

    /**
     * Get a pointer to the list of attributes.
     * @return A RetsXmlAttributeList pointer for the list of attributes.
     */
    RetsXmlAttributeListPtr GetAttributes();

    /**
     * For the named attribute, return the value.
     * @param name A string containing the name of the attribute.
     * @return A string containing the value for that attribute or an
     * empty string if the attribute does not exist.
     */
    std::string GetAttributeValue(std::string name);

    /**
     * Prints the object in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;

    /**
     * Checks to see if the element name and the vector of attributes 
     * are identical between two RetsXmlStartElementEvent objects.
     * @return TRUE if the names and attributes match.
     */
    virtual bool Equals(const RetsObject * rhs) const;

  private:
    typedef std::map<std::string, RetsXmlAttributePtr> AttributeMap;

    std::string mName;

    RetsXmlAttributeListPtr mAttributes;

    AttributeMap mAttributesByName;
};

};

/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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
#ifndef LIBRETS_RETS_XML_ATTRIBUTE_H
#define LIBRETS_RETS_XML_ATTRIBUTE_H
/** 
 * @file RetsXmlAttribute.h
 * (Internal) Contains the XML Parser Attribute interface class for use with libexpat.
 */
/// @cond MAINTAINER

#include <string>
#include "librets/RetsObject.h"

namespace librets {

/**
 * (Internal) RetsXmlAttribute is a class that contains an XML attribute and value.
 */
class RetsXmlAttribute : public RetsObject
{
  public:
    /**
     * Construct the attribute with the name and value.
     * @param name The name tag
     * @param value The value associated with the attribute.
     */
    RetsXmlAttribute(std::string name, std::string value);

    /**
     * Returns the name of the attribute.
     * @return string containing the name of the attribute.
     */
    std::string GetName() const;

    /**
     * Returns the value associated with the attribute.
     * @return string containing the value of the attribute.
     */
    std::string GetValue() const;

    /**
     * Prints the attribute and value in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;

    /**
     * Checks to see if the attribute and value are identical between
     * two RetsXmlAttribute objects.
     * @return TRUE if the name and value for both attributes matches.
     */
    virtual bool Equals(const RetsObject * rhs) const;

  private:
    std::string mName;
    std::string mValue;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

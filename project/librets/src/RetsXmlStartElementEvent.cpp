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
#include <ostream>
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlAttribute.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostream;

#define CLASS RetsXmlStartElementEvent

CLASS::CLASS()
    : mAttributes(new RetsXmlAttributeList())
{
}

CLASS::~CLASS()
{
}

RetsXmlEvent::Type CLASS::GetType() const
{
    return START_ELEMENT;
}

void CLASS::SetName(string name)
{
    mName = name;
}

string RetsXmlStartElementEvent::GetName() const
{
    return mName;
}

void RetsXmlStartElementEvent::AddAttribute(string name, string value)
{
    RetsXmlAttributePtr attribute(new RetsXmlAttribute(name, value));
    AddAttribute(attribute);
}

void RetsXmlStartElementEvent::AddAttribute(RetsXmlAttributePtr attribute)
{
    mAttributes->push_back(attribute);
    mAttributesByName[attribute->GetName()] = attribute;
}

string CLASS::GetAttributeValue(string name)
{
    RetsXmlAttributePtr attribute = mAttributesByName[name];
    if (attribute)
    {
        return attribute->GetValue();
    }
    else
    {
        return "";
    }
}

RetsXmlAttributeListPtr RetsXmlStartElementEvent::GetAttributes()
{
    return mAttributes;
}

ostream & RetsXmlStartElementEvent::Print(ostream & outputStream) const
{
    outputStream << "XML start element: <" << mName << ">, attributes [";
    RetsXmlAttributeList::iterator i;
    string separator = "";
    for (i = mAttributes->begin(); i != mAttributes->end(); i++)
    {
        RetsXmlAttributePtr attribute = *i;
        outputStream << separator << attribute->GetName() << "="
                << attribute->GetValue();
        separator = ", ";
    }
    outputStream << "]";
    return outputStream;
}

bool RetsXmlStartElementEvent::Equals(const RetsObject * object) const
{
    const RetsXmlStartElementEvent * rhs =
        dynamic_cast<const RetsXmlStartElementEvent *>(object);
    if (rhs == 0)
    {
        return false;
    }
    
    bool equals = true;
    equals &= (mName == rhs->mName);
    equals &= VectorEquals(*mAttributes, *rhs->mAttributes);
    return equals;
}

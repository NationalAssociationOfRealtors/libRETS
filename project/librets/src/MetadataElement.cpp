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

#include <iostream>
#include <boost/lexical_cast.hpp>
#include <boost/algorithm/string.hpp>                                           
#include "librets/MetadataElement.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::vector;
using boost::lexical_cast;
namespace ba = boost::algorithm;

#define CLASS MetadataElement

CLASS::CLASS()
{
}

CLASS::~CLASS()
{
}

string CLASS::GetStringAttribute(string attributeName,
                                 string defaultValue) const
{
    StringMap::const_iterator i = mAttributes.find(attributeName);
    if (i != mAttributes.end())
    {
        return i->second;
    }
    else
    {
        return defaultValue;
    }
}

int CLASS::GetIntAttribute(string attributeName, int defaultValue) const
{
    StringMap::const_iterator i = mAttributes.find(attributeName);
    if (i != mAttributes.end() && !isEmpty(i->second))
    {
        return lexical_cast<int>(i->second);
    }
    else
    {
        return defaultValue;
    }
}

bool CLASS::GetBoolAttribute(string attributeName, bool defaultValue) const
{
    StringMap::const_iterator i = mAttributes.find(attributeName);
    if (i != mAttributes.end())
    {
        string value = ba::to_lower_copy(i->second);
        return ((value == "1") || (value == "true"));
    }
    else
    {
        return defaultValue;
    }
}

void CLASS::SetAttribute(string attributeName, string attributeValue)
{
    mAttributes[attributeName] = attributeValue;
}

string CLASS::GetId() const
{
    return "";
}

string CLASS::GetLevel() const
{
    return mLevel;
}

void CLASS::SetLevel(string level)
{
    mLevel = level;
}

string CLASS::GetPath() const
{
    string id = GetId();
    if (!id.empty())
    {
        return join(mLevel, id, ":");
    }
    else
    {
        return "";
    }
}

std::ostream & CLASS::Print(std::ostream & outputStream) const
{
    return outputStream << "Type [" << GetType() << "], id [" << GetId()
                        << "], level [" << mLevel << "], path [" << GetPath()
                        << "]";
}


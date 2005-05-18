#include <iostream>
#include "librets/MetadataElement.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::vector;

#define CLASS MetadataElement

CLASS::CLASS()
{
}

CLASS::~CLASS()
{
}

CLASS::MetadataType MetadataElement::GetType() const
{
    return SYSTEM;
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
    if (i != mAttributes.end())
    {
        return intValue(i->second);
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
        return (bool) intValue(i->second);
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

vector<MetadataElementPtr> MetadataElement::GetChildren(MetadataType type)
{
    vector<MetadataElementPtr> children;
    return children;
}

string CLASS::GetLevel() const
{
    return mLevel;
}

void CLASS::SetLevel(string level)
{
    mLevel = level;
}

std::ostream & CLASS::Print(std::ostream & outputStream) const
{
    return outputStream << "Type [" << GetType() << "], level [" << mLevel
                        << "]";
}


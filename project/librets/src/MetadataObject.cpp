/*
 * Copyright (C) 2005-2014 National Association of REALTORS(R)
 * Portions Copyright (C) 2014 Real Estate Standards Organziation
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

#include "librets/MetadataObject.h"

using namespace librets;
using std::string;

#define CLASS MetadataObject

MetadataElement::MetadataType CLASS::GetType() const
{
    return OBJECT;
}

string CLASS::GetId() const
{
    return GetObjectType();
}

string CLASS::GetObjectType() const
{
    return GetStringAttribute("ObjectType");
}

string CLASS::GetMIMEType() const
{
    return GetStringAttribute("MIMEType");
}

string CLASS::GetVisibleName() const
{
    return GetStringAttribute("VisibleName");
}

string CLASS::GetDescription() const
{
    return GetStringAttribute("Description");
}

string CLASS::GetObjectTimeStamp() const
{
    return GetStringAttribute("ObjectTimeStamp");
}

string CLASS::GetObjectCount() const
{
    return GetStringAttribute("ObjectCount");
}

bool CLASS::GetLocationAvailability() const
{
    return GetBoolAttribute("LocationAvailability");
}

std::string CLASS::GetObjectData() const
{
    return GetStringAttribute("ObjectData");
}

int CLASS::GetMaxFileSize() const
{
    return GetIntAttribute("MaxFileSize");
}


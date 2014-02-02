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

#include "librets/MetadataClass.h"

using namespace librets;
using std::string;

#define CLASS_ MetadataClass

MetadataElement::MetadataType CLASS_::GetType() const
{
    return CLASS;
}

string CLASS_::GetId() const
{
    return CLASS_::GetClassName();
}

string CLASS_::GetClassName() const
{
    return GetStringAttribute("ClassName");
}

string CLASS_::GetStandardName() const
{
    return GetStringAttribute("StandardName");
}

string CLASS_::GetDescription() const
{
    return GetStringAttribute("Description");
}

std::string CLASS_::GetVisibleName() const
{
    return GetStringAttribute("VisibleName");
}

std::string CLASS_::GetTableVersion() const
{
    return GetStringAttribute("TableVersion");
}

std::string CLASS_::GetTableDate() const
{
    return GetStringAttribute("TableDate");
}

std::string CLASS_::GetUpdateVersion() const
{
    return GetStringAttribute("UpdateVersion");
}

std::string CLASS_::GetUpdateDate() const
{
    return GetStringAttribute("UpdateDate");
}

std::string CLASS_::GetClassTimeStamp() const
{
    return GetStringAttribute("ClassTimeStamp");
}

std::string CLASS_::GetDeletedFlagField() const
{
    return GetStringAttribute("DeletedFlagField");
}

std::string CLASS_::GetDeletedFlagValue() const
{
    return GetStringAttribute("DeletedFlagValue");
}

bool CLASS_::HasKeyIndex() const
{
    return GetBoolAttribute("HasKeyIndex");
}

bool CLASS_::HasOffsetSupport() const
{
    return GetBoolAttribute("OffsetSupport");
}

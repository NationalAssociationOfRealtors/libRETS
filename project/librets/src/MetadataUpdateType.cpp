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
#include "librets/MetadataUpdateType.h"

using namespace librets;
using std::string;

#define CLASS MetadataUpdateType

MetadataElement::MetadataType CLASS::GetType() const
{
    return UPDATE_TYPE;
}

std::string CLASS::GetSystemName() const
{
    return GetStringAttribute("SystemName");
}

int CLASS::GetSequence() const
{
    return GetIntAttribute("Sequence", 0);
}

std::string CLASS::GetAttributes() const
{
    return GetStringAttribute("Attributes");
}

std::string CLASS::GetDefault() const
{
    return GetStringAttribute("Default");
}

std::string CLASS::GetValidationExpressionID() const
{
    return GetStringAttribute("ValidationExpression");
}

std::string CLASS::GetValidationLookupName() const
{
    return GetStringAttribute("ValidationLookupName");
}

std::string CLASS::GetValidationExternalName() const
{
    return GetStringAttribute("ValidationExternalName");
}

int CLASS::GetMaxUpdate() const
{
    return GetIntAttribute("MaxUpdate", 0);
}
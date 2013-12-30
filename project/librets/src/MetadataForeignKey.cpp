/*
 * Copyright (C) 2005-2009 National Association of REALTORS(R)
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

#include "librets/MetadataForeignKey.h"

using namespace librets;
using std::string;

#define CLASS MetadataForeignKey

MetadataForeignKey::~MetadataForeignKey()
{
}

MetadataElement::MetadataType CLASS::GetType() const
{
    return FOREIGN_KEY;
}

string CLASS::GetID() const
{
    return GetForeignKeyID();
}

string CLASS::GetForeignKeyID() const
{
    return GetStringAttribute("ForeignKeyID");
}

string CLASS::GetParentResourceID() const
{
    return GetStringAttribute("ParentResourceID");
}

string CLASS::GetParentClassID() const
{
    return GetStringAttribute("ParentClassID");
}

string CLASS::GetParentSystemName() const
{
    return GetStringAttribute("ParentSystemName");
}

string CLASS::GetChildResourceID() const
{
    return GetStringAttribute("ChildResourceID");
}

string CLASS::GetChildClassID() const
{
    return GetStringAttribute("ChildClassID");
}

string CLASS::GetChildSystemName() const
{
    return GetStringAttribute("ChildSystemName");
}

string CLASS::GetConditionalParentField() const
{
    return GetStringAttribute("ConditionalParentField");
}

string CLASS::GetConditionalParentValue() const
{
    return GetStringAttribute("ConditionalParentValue");
}

bool CLASS::GetOneToManyFlag() const
{
    return GetBoolAttribute("OneToManyFlag");
}


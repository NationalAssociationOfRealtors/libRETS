/*
 * Copyright (C) 2014 Real Estate Standards Organization
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
#include "librets/MetadataColumnGroup.h"

using namespace librets;
using std::string;

MetadataElement::MetadataType MetadataColumnGroup::GetType() const
{
    return COLUMN_GROUP;
}

std::string MetadataColumnGroup::GetId() const
{
    return MetadataColumnGroup::GetColumnGroupName();
}

std::string MetadataColumnGroup::GetColumnGroupName() const
{
    return GetStringAttribute("ColumnGroupName");
}

std::string MetadataColumnGroup::GetControlSystemName() const
{
    return GetStringAttribute("ControlSystemName");
}

std::string MetadataColumnGroup::GetLongName() const
{
    return GetStringAttribute("LongName");
}

std::string MetadataColumnGroup::GetShortName() const
{
    return GetStringAttribute("ShortName");
}

std::string MetadataColumnGroup::GetDescription() const
{
    return GetStringAttribute("Description");
}

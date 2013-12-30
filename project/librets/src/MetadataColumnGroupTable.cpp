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
#include "librets/MetadataColumnGroupTable.h"

using namespace librets;
using std::string;

MetadataElement::MetadataType MetadataColumnGroupTable::GetType() const
{
    return COLUMN_GROUP_TABLE;
}

std::string MetadataColumnGroupTable::GetSystemName() const
{
    return GetStringAttribute("SystemName");
}

std::string MetadataColumnGroupTable::GetId() const
{
    return MetadataColumnGroupTable::GetColumnGroupSetName();
}

std::string MetadataColumnGroupTable::GetColumnGroupSetName() const
{
    return GetStringAttribute("ColumnGroupSetName");
}

std::string MetadataColumnGroupTable::GetLongName() const
{
    return GetStringAttribute("LongName");
}

std::string MetadataColumnGroupTable::GetShortName() const
{
    return GetStringAttribute("ShortName");
}

int MetadataColumnGroupTable::GetDisplayOrder() const
{
    return GetIntAttribute("DisplayOrder");
}

int MetadataColumnGroupTable::GetDisplayLength() const
{
    return GetIntAttribute("DisplayLength");
}

int MetadataColumnGroupTable::GetDisplayHeight() const
{
    return GetIntAttribute("DisplayHeight");
}

bool MetadataColumnGroupTable::GetImmediateRefresh() const
{
    return GetBoolAttribute("ImmediateRefresh");
}

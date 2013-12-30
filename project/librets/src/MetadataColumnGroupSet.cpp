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
#include "librets/MetadataColumnGroupSet.h"

using namespace librets;
using std::string;

MetadataElement::MetadataType MetadataColumnGroupSet::GetType() const
{
    return COLUMN_GROUP_SET;
}

std::string MetadataColumnGroupSet::GetId() const
{
    return MetadataColumnGroupSet::GetColumnGroupSetName();
}

std::string MetadataColumnGroupSet::GetColumnGroupSetName() const
{
    return GetStringAttribute("ColumnGroupSetName");
}

std::string MetadataColumnGroupSet::GetColumnGroupSetParent() const
{
    return GetStringAttribute("ColumnGroupSetParent");
}

int MetadataColumnGroupSet::GetSequence() const
{
    return GetIntAttribute("Sequence");
}

std::string MetadataColumnGroupSet::GetLongName() const
{
    return GetStringAttribute("LongName");
}

std::string MetadataColumnGroupSet::GetShortName() const
{
    return GetStringAttribute("ShortName");
}

std::string MetadataColumnGroupSet::GetDescription() const
{
    return GetStringAttribute("Description");
}

std::string MetadataColumnGroupSet::GetColumnGroupName() const
{
    return GetStringAttribute("ColumnGroupName");
}

MetadataColumnGroupSet::PresentationStyle MetadataColumnGroupSet::GetPresentationStyle() const
{
    std::string style =  GetStringAttribute("ColumnGroupSetName");

    if (style == "Edit")
    {
        return EDIT;
    }
    else if (style == "Matrix")
    {
        return MATRIX;
    }
    else if (style == "List")
    {
        return LIST;
    }
    else if (style == "Edit List")
    {
        return EDIT_LIST;
    }
    else if (style == "GIS Map Search")
    {
        return GIS_MAP_SEARCH;
    }
    else if (style == "URL")
    {
        return URL;
    }
    else
    {
        return NO_PRESENTATION;
    }
}

std::string MetadataColumnGroupSet::GetURL() const
{
    return GetStringAttribute("URL");
}

std::string MetadataColumnGroupSet::GetForeignKeyID() const
{
    return GetStringAttribute("ForeignKeyID");
}


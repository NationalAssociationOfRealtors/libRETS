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

#include "librets/MetadataFilter.h"

using namespace librets;
using std::string;

MetadataFilter::~MetadataFilter()
{
}

MetadataElement::MetadataType MetadataFilter::GetType() const
{
    return FILTER;
}

string MetadataFilter::GetID() const
{
    return GetFilterID();
}

string MetadataFilter::GetFilterID() const
{
    return GetStringAttribute("FilterID");
}

string MetadataFilter::GetParentResourceID() const
{
    return GetStringAttribute("ParentResource");
}

string MetadataFilter::GetParentLookupName() const
{
    return GetStringAttribute("ParentLookupName");
}

string MetadataFilter::GetChildResourceID() const
{
    return GetStringAttribute("ChildResource");
}

string MetadataFilter::GetChildLookupName() const
{
    return GetStringAttribute("ChildLookupName");
}

bool MetadataFilter::GetNotShownByDefault() const
{
    return GetBoolAttribute("NotShownByDefault");
}


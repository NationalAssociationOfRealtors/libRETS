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

#include "librets/MetadataFilterType.h"

using namespace librets;
using std::string;

MetadataFilterType::~MetadataFilterType()
{
}

MetadataElement::MetadataType MetadataFilterType::GetType() const
{
    return FILTER_TYPE;
}

string MetadataFilterType::GetID() const
{
    return GetFilterTypeID();
}

string MetadataFilterType::GetFilterTypeID() const
{
    return GetStringAttribute("FilterTypeID");
}

string MetadataFilterType::GetParentValue() const
{
    return GetStringAttribute("ParenValue");
}

string MetadataFilterType::GetChildValue() const
{
    return GetStringAttribute("ChildValue");
}

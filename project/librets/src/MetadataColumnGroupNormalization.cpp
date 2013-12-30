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
#include "librets/MetadataColumnGroupNormalization.h"

using namespace librets;
using std::string;

MetadataElement::MetadataType MetadataColumnGroupNormalization::GetType() const
{
    return COLUMN_GROUP_NORMALIZATION;
}

std::string MetadataColumnGroupNormalization::GetId() const
{
    return GetStringAttribute("ColumnGroup");
}

std::string MetadataColumnGroupNormalization::GetTypeIdentifier() const
{
    return GetStringAttribute("TypeIdentifier");
}

int MetadataColumnGroupNormalization::GetSequence() const
{
    return GetIntAttribute("Sequence");
}

std::string MetadataColumnGroupNormalization::GetColumnLabel() const
{
    return GetStringAttribute("ColumnLabel");
}

std::string MetadataColumnGroupNormalization::GetSystemName() const
{
    return GetStringAttribute("SystemName");
}


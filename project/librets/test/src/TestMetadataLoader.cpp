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

#include "testUtil.h"
#include "TestMetadataLoader.h"
#include "librets/DefaultMetadataCollector.h"

using namespace librets;
using namespace librets::util;
using std::string;

#define CLASS TestMetadataLoader

CLASS::CLASS()
    : mPrivateCollector(new DefaultMetadataCollector()), mTotalLoadCount(0)
{
}

void CLASS::SetCollector(MetadataElementCollectorPtr collector)
{
    mCollector = collector;
}

void CLASS::LoadMetadata(MetadataElement::Type type, std::string level)
{
    MetadataElementListPtr elements = mPrivateCollector->FindByLevel(type,
                                                                     level);
    MetadataElementList::const_iterator i;
    for (i = elements->begin(); i != elements->end(); i++)
    {
        MetadataElementPtr element = *i;
        mCollector->AddElement(element);
    }
    mLoadCounter[type] = mLoadCounter[type] + 1;
    mTotalLoadCount++;
}

int CLASS::GetLoadCount(MetadataElement::Type type)
{
    return mLoadCounter[type];
}

int CLASS::GetTotalLoadCount()
{
    return mTotalLoadCount;
}

void CLASS::AddElement(MetadataElementPtr element)
{
    mPrivateCollector->AddElement(element);
}

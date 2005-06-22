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
#include "librets/MetadataByLevelCollector.h"

using namespace librets;
using std::string;

#define CLASS MetadataByLevelCollector

CLASS::CLASS()
    : mTypeMap()
{
}

CLASS::~CLASS()
{
}

void CLASS::AddElement(MetadataElementPtr element)
{
    string level = element->GetLevel();
    MetadataElement::MetadataType type = element->GetType();
    MetadataElementListPtr elements;
    LevelMapPtr levelMap;
    TypeMap::iterator i = mTypeMap.find(type);
    if (i == mTypeMap.end())
    {
        levelMap.reset(new LevelMap());
        mTypeMap[type] = levelMap;
    }
    else
    {
        levelMap = i->second;
    }

    LevelMap::iterator j = levelMap->find(level);
    if (j == levelMap->end())
    {
        elements.reset(new MetadataElementList());
        (*levelMap)[level] = elements;
    }
    else
    {
        elements = j->second;
    }

    elements->push_back(element);
}
    
MetadataElementListPtr CLASS::Find(MetadataElement::MetadataType type,
                                   string level)
{
    MetadataElementListPtr elements;

    TypeMap::iterator i = mTypeMap.find(type);
    if (i == mTypeMap.end())
    {
        elements.reset(new MetadataElementList());
        return elements;
    }

    LevelMapPtr levelMap = i->second;
    LevelMap::iterator j = levelMap->find(level);
    if (j == levelMap->end())
    {
        elements.reset(new MetadataElementList());
        return elements;
    }

    elements = j->second;
    return elements;
}

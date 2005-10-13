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

#include <iostream>
#include "librets/DefaultMetadataCollector.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::endl;

#define CLASS DefaultMetadataCollector

CLASS::CLASS()
    : mTypeMapByLevel(), mTypeMapByPath()
{
}

CLASS::~CLASS()
{
}

void CLASS::AddElement(MetadataElementPtr element)
{
    string level = element->GetLevel();
    MetadataElement::MetadataType type = element->GetType();
    LevelElementsPtr levelMap = GetLevelElementsByType(type);
    MetadataElementListPtr elements = GetElementsByLevel(levelMap, level);
    elements->push_back(element);
    
    string path = element->GetPath();
    if (!path.empty())
    {
        PathElementsPtr pathElements = GetPathElementsByType(type);
        (*pathElements)[path] = element;
    }
}

CLASS::LevelElementsPtr CLASS::GetLevelElementsByType(
    MetadataElement::MetadataType type)
{
    LevelElementsPtr levelElements;
    TypeMapByLevel::iterator i = mTypeMapByLevel.find(type);
    if (i == mTypeMapByLevel.end())
    {
        levelElements.reset(new LevelElements());
        mTypeMapByLevel[type] = levelElements;
    }
    else
    {
        levelElements = i->second;
    }
    return levelElements;
}

MetadataElementListPtr CLASS::GetElementsByLevel(LevelElementsPtr levelElements,
                                                 string level)
{
    MetadataElementListPtr elements;
    LevelElements::iterator i = levelElements->find(level);
    if (i == levelElements->end())
    {
        elements.reset(new MetadataElementList());
        (*levelElements)[level] = elements;
    }
    else
    {
        elements = i->second;
    }
    return elements;
}

CLASS::PathElementsPtr CLASS::GetPathElementsByType(
    MetadataElement::MetadataType type)
{
    PathElementsPtr pathElements;
    TypeMapByPath::iterator i = mTypeMapByPath.find(type);
    if (i == mTypeMapByPath.end())
    {
        pathElements.reset(new PathElements());
        mTypeMapByPath[type] = pathElements;
    }
    else
    {
        pathElements = i->second;
    }
    return pathElements;
}

std::ostream & CLASS::Print(std::ostream & outputStream) const
{
    outputStream << "By level:" << endl;
    TypeMapByLevel::const_iterator i;
    for (i = mTypeMapByLevel.begin(); i != mTypeMapByLevel.end(); i++)
    {
        outputStream << "  Type: " << i->first << endl;
        LevelElementsPtr levelElements = i->second;
        LevelElements::const_iterator j;
        for (j = levelElements->begin(); j != levelElements->end(); j++)
        {
            outputStream << "  " << j->first << " -> " << Output(*j->second)
                         << endl;
        }
    }
    outputStream << endl;
    
    outputStream << "By path:" << endl;
    TypeMapByPath::const_iterator j;
    for (j = mTypeMapByPath.begin(); j != mTypeMapByPath.end(); j++)
    {
        outputStream << "  Type: " << j->first << endl;
        PathElementsPtr pathElements = j->second;
        PathElements::const_iterator k;
        for (k = pathElements->begin(); k != pathElements->end(); k++)
        {
            outputStream << "  " << k->first << " -> " << Output(*k->second)
                         << endl;
        }
    }
    outputStream << endl;

    return outputStream;
}

MetadataElementListPtr CLASS::FindByLevel(MetadataElement::MetadataType type,
                                          string level)
{
    LevelElementsPtr levelElements = GetLevelElementsByType(type);
    MetadataElementListPtr elements = GetElementsByLevel(levelElements, level);
    return elements;
}

MetadataElementPtr CLASS::FindByPath(MetadataElement::MetadataType type,
                                     string level, string id)
{
    string path = join(level, id, ":");
    MetadataElementPtr element;
    PathElementsPtr pathElements = GetPathElementsByType(type);
    PathElements::iterator i = pathElements->find(path);
    if (i != pathElements->end())
    {
        element = i->second;
    }
    return element;
}

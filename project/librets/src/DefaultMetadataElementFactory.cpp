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
#include <stdarg.h>
#include <vector>
#include <sstream>
#include "librets/DefaultMetadataElementFactory.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/MetadataUpdate.h"
#include "librets/MetadataUpdateType.h"
#include "librets/MetadataUpdateHelp.h"
#include "librets/MetadataObject.h"
#include "librets/MetadataSearchHelp.h"
#include "librets/MetadataEditMask.h"
#include "librets/MetadataLookup.h"
#include "librets/MetadataLookupType.h"
#include "librets/MetadataValidationLookup.h"
#include "librets/MetadataValidationLookupType.h"
#include "librets/MetadataValidationExternal.h"
#include "librets/MetadataValidationExternalType.h"
#include "librets/MetadataValidationExpression.h"
#include "librets/MetadataForeignKey.h"
#include "librets/RetsErrorHandler.h"

using namespace librets;
using std::string;
using std::map;
using std::vector;
using std::ostringstream;

#define CLASS DefaultMetadataElementFactory

char * CLASS::SYSTEM_ATTRIBUTES[] = {0};
char * CLASS::RESOURCE_ATTRIBUTES[] = {0};
char * CLASS::CLASS_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::TABLE_ATTRIBUTES[] = {"Resource", "Class", 0};
char * CLASS::UPDATE_ATTRIBUTES[] = {"Resource", "Class", 0};
char * CLASS::UPDATE_TYPE_ATTRIBUTES[] = {"Resource", "Class", "Update", 0};
char * CLASS::UPDATE_HELP_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::OBJECT_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::SEARCH_HELP_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::EDIT_MASK_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::LOOKUP_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::LOOKUP_TYPE_ATTRIBUTES[] = {"Resource", "Lookup", 0};
char * CLASS::VALIDATION_LOOKUP_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::VALIDATION_LOOKUP_TYPE_ATTRIBUTES[] =
    {"Resource", "ValidationLookup", 0};
char * CLASS::VALIDATION_EXTERNAL_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::VALIDATION_EXTERNAL_TYPE_ATTRIBUTES[] =
    {"Resource", "ValidationExternal", 0};
char * CLASS::VALIDATION_EXPRESSION_ATTRIBUTES[] = {"Resource", 0};
char * CLASS::FOREIGN_KEY_ATTRIBUTES[] = {0};

CLASS::CLASS()
    : mTypeMap(), mCreatorMap(), mAttributeMap()
{
    AddMappings("METADATA-SYSTEM",  &CLASS::CreateSystem, SYSTEM_ATTRIBUTES);
    AddMappings("METADATA-RESOURCE", &CLASS::CreateResource,
                RESOURCE_ATTRIBUTES);
    AddMappings("METADATA-CLASS", &CLASS::CreateClass, CLASS_ATTRIBUTES);
    AddMappings("METADATA-TABLE", &CLASS::CreateTable, TABLE_ATTRIBUTES);
    AddMappings("METADATA-UPDATE", &CLASS::CreateUpdate, UPDATE_ATTRIBUTES);
    AddMappings("METADATA-UPDATE_TYPE", &CLASS::CreateUpdateType,
                UPDATE_TYPE_ATTRIBUTES);
    AddMappings("METADATA-UPDATE_HELP", &CLASS::CreateUpdateHelp,
                UPDATE_HELP_ATTRIBUTES);
    AddMappings("METADATA-OBJECT", &CLASS::CreateObject, OBJECT_ATTRIBUTES);
    AddMappings("METADATA-SEARCH_HELP", &CLASS::CreateSearchHelp,
                SEARCH_HELP_ATTRIBUTES);
    // Gotta love the consistency here...
    AddMappings("METADATA-EDITMASK", &CLASS::CreateEditMask,
                EDIT_MASK_ATTRIBUTES);
    AddMappings("METADATA-LOOKUP", &CLASS::CreateLookup, LOOKUP_ATTRIBUTES);
    AddMappings("METADATA-LOOKUP_TYPE", &CLASS::CreateLookupType,
                LOOKUP_TYPE_ATTRIBUTES);
    AddMappings("METADATA-VALIDATION_LOOKUP", &CLASS::CreateValidationLookup,
                VALIDATION_LOOKUP_ATTRIBUTES);
    AddMappings("METADATA-VALIDATION_LOOKUP_TYPE",
                &CLASS::CreateValidationLookupType,
                VALIDATION_LOOKUP_TYPE_ATTRIBUTES);
    AddMappings("METADATA-VALIDATION_EXTERNAL",
                &CLASS::CreateValidationExternal,
                VALIDATION_EXTERNAL_ATTRIBUTES);
    AddMappings("METADATA-VALIDATION_EXTERNAL_TYPE",
                &CLASS::CreateValidationExternalType,
                VALIDATION_EXTERNAL_TYPE_ATTRIBUTES);
    AddMappings("METADATA-VALIDATION_EXPRESSION",
                &CLASS::CreateValidationExpression,
                VALIDATION_EXPRESSION_ATTRIBUTES);
    AddMappings("METADATA-FOREIGNKEYS", &CLASS::CreateForeignKey,
                FOREIGN_KEY_ATTRIBUTES);
    AddMappings("METADATA-FOREIGN_KEYS", &CLASS::CreateForeignKey,
                FOREIGN_KEY_ATTRIBUTES);
}

void CLASS::AddMappings(string name, CreatorPointer creator,
                        char ** attributes)
{
    mCreatorMap[name] = creator;
    mAttributeMap[name] = attributes;
}

CLASS::~CLASS()
{
}

void CLASS::SetErrorHandler(RetsErrorHandler * errorHandler)
{
    mErrorHandler = errorHandler;
}

string CLASS::LevelBuilder(RetsXmlStartElementEventPtr startEvent,
                           char ** attributes)
{
    ostringstream level;
    string separator = "";
    char ** attribute;
    for (attribute = attributes; *attribute != 0; attribute++)
    {
        level << separator << startEvent->GetAttributeValue(*attribute);
        separator = ":";
    }
    return level.str();
}

MetadataElementPtr CLASS::CreateSystem(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataSystem());
    return element;
}

MetadataElementPtr CLASS::CreateResource(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataResource());
    return element;
}

MetadataElementPtr CLASS::CreateClass(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataClass());
    return element;
}

MetadataElementPtr CLASS::CreateTable(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataTable());
    return element;
}

MetadataElementPtr CLASS::CreateUpdate(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataUpdate());
    return element;
}

MetadataElementPtr CLASS::CreateUpdateType(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataUpdateType());
    return element;
}

MetadataElementPtr CLASS::CreateUpdateHelp(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataUpdateHelp());
    return element;
}

MetadataElementPtr CLASS::CreateObject(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataObject());
    return element;
}

MetadataElementPtr CLASS::CreateSearchHelp(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataSearchHelp());
    return element;
}

MetadataElementPtr CLASS::CreateEditMask(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataEditMask());
    return element;
}

MetadataElementPtr CLASS::CreateLookup(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataLookup());
    return element;
}

MetadataElementPtr CLASS::CreateLookupType(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataLookupType());
    return element;
}

MetadataElementPtr CLASS::CreateValidationLookup(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataValidationLookup());
    return element;
}

MetadataElementPtr CLASS::CreateValidationLookupType(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataValidationLookupType());
    return element;
}

MetadataElementPtr CLASS::CreateValidationExternal(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataValidationExternal());
    return element;
}

MetadataElementPtr CLASS::CreateValidationExternalType(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataValidationExternalType());
    return element;
}

MetadataElementPtr CLASS::CreateValidationExpression(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataValidationExpression());
    return element;
}

MetadataElementPtr CLASS::CreateForeignKey(
        RetsXmlStartElementEventPtr startEvent)
{
    MetadataElementPtr element(new MetadataForeignKey());
    return element;
}

MetadataElementPtr CLASS::CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent)
{
    MetadataElementPtr element;
    string name = startElementEvent->GetName();
    CreatorMap::iterator creator = mCreatorMap.find(name);
    if (creator != mCreatorMap.end())
    {
        CreatorPointer creatorPointer = creator->second;
        element = (this->*creatorPointer)(startElementEvent);
        AttributeMap::iterator i = mAttributeMap.find(name);
        string level = LevelBuilder(startElementEvent, mAttributeMap[name]);
        element->SetLevel(level);
    }
    else
    {
        mErrorHandler->HandleUnknownMetadata(name);
    }
    return element;
}

void CLASS::AddElement(MetadataElementPtr element, string level)
{
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

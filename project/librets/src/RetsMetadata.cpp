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
#include <sstream>
#include "librets/RetsMetadata.h"
#include "librets/MetadataByLevelCollector.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/RetsException.h"

using namespace librets;
using std::string;
using std::vector;
using std::ostringstream;
namespace b = boost;

RetsMetadata::RetsMetadata(MetadataByLevelCollectorPtr collector)
{
    mCollector = collector;
    InitSystem();
    InitAllResources();
    InitAllClasses();
}

void RetsMetadata::InitSystem()
{
    MetadataElementListPtr elements =
        mCollector->FindByLevel(MetadataElement::SYSTEM, "");
    if (elements->size() != 1)
    {
        ostringstream message;
        message << "Expecting 1 system metadata element, found "
                << elements->size();
        throw RetsException(message.str());
    }
    MetadataElementPtr element = elements->at(0);
    mSystem = dynamic_cast<MetadataSystem *>(element.get());
}

void RetsMetadata::InitAllClasses()
{
    mAllClasses.reset(new MetadataClassList());
    MetadataElementListPtr resources =
        mCollector->FindByLevel(MetadataElement::RESOURCE, "");
    MetadataElementList::iterator i;
    for (i = resources->begin(); i != resources->end(); i++)
    {
        MetadataResourcePtr resource =
            b::dynamic_pointer_cast<MetadataResource>(*i);
        string level = resource->GetResourceID();

        MetadataElementListPtr classes =
            mCollector->FindByLevel(MetadataElement::CLASS, level);
        MetadataElementList::iterator j;
        for (j = classes->begin(); j != classes->end(); j++)
        {
            MetadataClass * aClass =
                dynamic_cast<MetadataClass *>((*j).get());
            mAllClasses->push_back(aClass);
        }
    }
}

void RetsMetadata::InitAllResources()
{
    mAllResources.reset(new MetadataResourceList());
    MetadataElementListPtr resources =
        mCollector->FindByLevel(MetadataElement::RESOURCE, "");
    MetadataElementList::iterator i;
    for (i = resources->begin(); i != resources->end(); i++)
    {
        MetadataResource * resource =
            dynamic_cast<MetadataResource *>((*i).get());
        mAllResources->push_back(resource);
    }
}

MetadataSystem * RetsMetadata::GetSystem() const
{
    return mSystem;
}

MetadataResourceList RetsMetadata::GetAllResources() const
{
    return *mAllResources;
}

MetadataResource * RetsMetadata::GetResource(string resourceName) const
{
    MetadataResource * metadataResource = 0;
    
    bool found = false;
    MetadataResourceList::iterator i = mAllResources->begin();
    
    while (i != mAllResources->end() && !found)
    {
        MetadataResource * res = *i;
        string testName = res->GetResourceID();
        if (testName == resourceName)
        {
            metadataResource = res;
            found = true;
        }
        i++;
    }
    
    return metadataResource;
}

MetadataClassList RetsMetadata::GetAllClasses() const
{
    return *mAllClasses;
}

MetadataClass * RetsMetadata::GetClass(string resourceName, string className)
    const
{
    MetadataClass * metadataClass = 0;

    MetadataClassList classList = GetClassesForResource(resourceName);
    
    bool found = false;
    MetadataClassList::iterator i = classList.begin();
    
    while (i != classList.end() && !found)
    {
        MetadataClass * clazz = *i;
        string testName = clazz->GetClassName();
        if (testName == className)
        {
            metadataClass = clazz;
            found = true;
        }
        i++;
    }
    
    return metadataClass;
}

MetadataClassList RetsMetadata::GetClassesForResource(
    string resourceName) const
{
    MetadataElementListPtr elements =
    mCollector->FindByLevel(MetadataElement::CLASS, resourceName);
    
    MetadataClassList classes;
    MetadataElementList::iterator i;
    for (i = elements->begin(); i != elements->end(); i++)
    {
        MetadataClass * aClass = dynamic_cast<MetadataClass *>((*i).get());
        classes.push_back(aClass);
    }
    
    return classes;
}

MetadataTableList RetsMetadata::GetTablesForClass(
    MetadataClass * metadataClass) const
{
    return GetTablesForClass(metadataClass->GetLevel(),
                             metadataClass->GetClassName());
}

MetadataTableList RetsMetadata::GetTablesForClass(
    string resourceName, string className) const
{
    string level = resourceName + ":" + className;
    MetadataElementListPtr elements =
        mCollector->FindByLevel(MetadataElement::TABLE, level);
    
    MetadataTableList tables;
    MetadataElementList::iterator i;
    for (i = elements->begin(); i != elements->end(); i++)
    {
        MetadataTable * table = dynamic_cast<MetadataTable *>((*i).get());
        tables.push_back(table);
    }
    return tables;
}

MetadataTable * RetsMetadata::GetTable(string resourceName, string className,
                                        string tableName) const
{
    MetadataTable * metadataTable = 0;

    MetadataTableList tableList =
        GetTablesForClass(resourceName, className);
    
    bool found = false;
    MetadataTableList::iterator i = tableList.begin();
    
    while (i != tableList.end() && !found)
    {
        MetadataTable * table = *i;
        string testName = table->GetSystemName();
        if (testName == tableName)
        {
            metadataTable = table;
            found = true;
        }
        i++;
    }
    
    return metadataTable;
}

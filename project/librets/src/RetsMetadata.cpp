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
#include <algorithm>
#include "librets/RetsMetadata.h"
#include "librets/MetadataFinder.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/RetsException.h"

using namespace librets;
using std::string;
using std::vector;
using std::ostringstream;
using std::find_if;
namespace b = boost;

#define CLASS_ RetsMetadata

template<typename DERIVED>
struct ElementVectorCast
{
    typedef std::vector< DERIVED * > derived_vec_type;
    typedef boost::shared_ptr< derived_vec_type > derived_vec_type_ptr;
    
    derived_vec_type_ptr operator()(const MetadataElementList & base_vec)
    {
        derived_vec_type_ptr derived_vec(new derived_vec_type());
        MetadataElementList::const_iterator i;
        for (i = base_vec.begin(); i != base_vec.end(); i++)
        {
            MetadataElement * element = (*i).get();
            DERIVED * derived = dynamic_cast< DERIVED * >(element);
            derived_vec->push_back(derived);
        }
        return derived_vec;
    }
};

RetsMetadata::RetsMetadata(MetadataFinderPtr finder)
    :  mFinder(finder)
{
    InitSystem();
}

void RetsMetadata::InitSystem()
{
    MetadataElementListPtr elements =
        mFinder->FindByLevel(MetadataElement::SYSTEM, "");
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

MetadataSystem * RetsMetadata::GetSystem() const
{
    return mSystem;
}

MetadataResourceList RetsMetadata::GetAllResources() const
{
    MetadataElementListPtr elements =
        mFinder->FindByLevel(MetadataElement::RESOURCE, "");
    
    ElementVectorCast<MetadataResource> cast;
    MetadataResourceListPtr resources =  cast(*elements);
    return *resources;
}

MetadataResource * RetsMetadata::GetResource(string resourceName) const
{
    MetadataResource * metadataResource = 0;
    MetadataElementListPtr elements =
        mFinder->FindByLevel(MetadataElement::RESOURCE, "");
    MetadataElementList::iterator i;
    i = find_if(elements->begin(), elements->end(),
                MetadataElementIdEqual(resourceName));
    if (i != elements->end())
        metadataResource = dynamic_cast<MetadataResource *>((*i).get());
    return metadataResource;
}

MetadataClass * RetsMetadata::GetClass(string resourceName, string className)
    const
{
    MetadataClass * metadataClass = 0;
    MetadataElementListPtr elements =
        mFinder->FindByLevel(MetadataElement::CLASS, resourceName);

    MetadataElementList::iterator i;
    i = find_if(elements->begin(), elements->end(),
                MetadataElementIdEqual(className));
    if (i != elements->end())
        metadataClass = dynamic_cast<MetadataClass *>((*i).get());
    
    return metadataClass;
}

MetadataClassList RetsMetadata::GetClassesForResource(
    string resourceName) const
{
    MetadataElementListPtr elements =
        mFinder->FindByLevel(MetadataElement::CLASS, resourceName);

    ElementVectorCast<MetadataClass> cast;
    MetadataClassListPtr classes = cast(*elements);
    return *classes;
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
        mFinder->FindByLevel(MetadataElement::TABLE, level);

    ElementVectorCast<MetadataTable> cast;
    MetadataTableListPtr tables = cast(*elements);
    return *tables;
}

MetadataTable * RetsMetadata::GetTable(string resourceName, string className,
                                        string tableName) const
{
    string level = resourceName + ":" + className;
    MetadataTable * metadataTable = 0;
    MetadataElementListPtr elements =
        mFinder->FindByLevel(MetadataElement::TABLE, level);
    
    MetadataElementList::iterator i;
    i = find_if(elements->begin(), elements->end(),
                MetadataElementIdEqual(tableName));
    if (i != elements->end())
        metadataTable = dynamic_cast<MetadataTable *>((*i).get());
    
    return metadataTable;
}

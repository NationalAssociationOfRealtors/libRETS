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
#include "librets/MetadataLookup.h"
#include "librets/RetsException.h"

using namespace librets;
using std::string;
using std::vector;
using std::ostringstream;
using std::find_if;
namespace b = boost;

#define CLASS_ RetsMetadata

template<typename DERIVED>
class FinderHelper
{
  public:
    typedef std::vector< DERIVED * > derived_vec_type;
    typedef boost::shared_ptr< derived_vec_type > derived_vec_type_ptr;
    
    FinderHelper(MetadataFinderPtr finder) : mFinder(finder) { }
    
    derived_vec_type_ptr cast(const MetadataElementList & base_vec)
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
    
    derived_vec_type_ptr FindByLevel(MetadataElement::Type type, string level)
    {
        MetadataElementListPtr elements = mFinder->FindByLevel(type, level);
        derived_vec_type_ptr derivedElements = cast(*elements);
        return derivedElements;
    }
    
    DERIVED * FindByPath(MetadataElement::Type type, string level, string id)
    {
        MetadataElementPtr element = mFinder->FindByPath(type, level, id);
        return dynamic_cast< DERIVED *>(element.get());
    }
    
  private:
    MetadataFinderPtr mFinder;
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
    FinderHelper<MetadataResource> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::RESOURCE, "");
}

MetadataResource * RetsMetadata::GetResource(string resourceName) const
{
    FinderHelper<MetadataResource> helper(mFinder);
    return helper.FindByPath(MetadataElement::RESOURCE, "", resourceName);
}

MetadataClass * RetsMetadata::GetClass(string resourceName, string className)
    const
{
    FinderHelper<MetadataClass> helper(mFinder);
    return helper.FindByPath(MetadataElement::CLASS, resourceName, className);
}

MetadataClassList RetsMetadata::GetAllClasses(
    string resourceName) const
{
    FinderHelper<MetadataClass> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::CLASS, resourceName);
}

MetadataTableList RetsMetadata::GetAllTables(
    MetadataClass * metadataClass) const
{
    return GetAllTables(metadataClass->GetLevel(),
                        metadataClass->GetClassName());
}

MetadataTableList RetsMetadata::GetAllTables(
    string resourceName, string className) const
{
    string level = resourceName + ":" + className;
    FinderHelper<MetadataTable> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::TABLE, level);
}

MetadataTable * RetsMetadata::GetTable(string resourceName, string className,
                                        string tableName) const
{
    string level = resourceName + ":" + className;
    FinderHelper<MetadataTable> helper(mFinder);
    return helper.FindByPath(MetadataElement::TABLE, level, tableName);
}

MetadataLookupList RetsMetadata::GetAllLookups(string resourceName) const
{
    FinderHelper<MetadataLookup> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::LOOKUP, resourceName);
}

MetadataLookup * RetsMetadata::GetLookup(std::string resourceName,
                                         std::string lookupName) const
{
    FinderHelper<MetadataLookup> helper(mFinder);
    return helper.FindByPath(MetadataElement::LOOKUP, resourceName, lookupName);
}

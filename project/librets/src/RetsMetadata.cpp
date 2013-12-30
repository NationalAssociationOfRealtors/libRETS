/*
 * Copyright (C) 2005-2009 National Association of REALTORS(R)
 * Portions Copyright (C) 2014 Real Estate Standards Organziation
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
#include <boost/algorithm/string.hpp>

#include "librets/RetsMetadata.h"
#include "librets/DefaultMetadataCollector.h"
#include "librets/MetadataFinder.h"
#include "librets/MetadataForeignKey.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/MetadataLookup.h"
#include "librets/MetadataLookupType.h"
#include "librets/MetadataObject.h"
#include "librets/MetadataSearchHelp.h"
#include "librets/MetadataColumnGroup.h"
#include "librets/MetadataColumnGroupNormalization.h"
#include "librets/MetadataColumnGroupTable.h"
#include "librets/MetadataColumnGroupControl.h"
#include "librets/MetadataColumnGroupSet.h"
#include "librets/MetadataFilter.h"
#include "librets/MetadataFilterType.h"
#include "librets/RetsException.h"
#include "librets/XmlMetadataParser.h"

using namespace librets;
using std::string;
using std::vector;
using std::ostringstream;
using std::find_if;
namespace b = boost;
namespace ba = boost::algorithm;

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

RetsMetadata * RetsMetadata::CreateAndParse(
                                istreamPtr inputStream,
                                EncodingType encoding,
                                ExceptionErrorHandler * handler)
{
    DefaultMetadataCollectorPtr collector(new DefaultMetadataCollector());
    XmlMetadataParserPtr parser(new XmlMetadataParser(collector, handler));
    
    parser->SetEncoding(encoding);
    parser->Parse(inputStream);
    return new RetsMetadata(collector);
}

RetsMetadata * RetsMetadata::CreateAndParse(
                                BinaryData binaryData,
                                EncodingType encoding,
                                ExceptionErrorHandler * handler)
{
    istreamPtr inputStream(new std::istringstream(binaryData.AsString()));
    return CreateAndParse(inputStream, encoding, handler);
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

MetadataForeignKeyList RetsMetadata::GetAllForeignKeys() const
{
   FinderHelper<MetadataForeignKey> helper(mFinder);
   return *helper.FindByLevel(MetadataElement::FOREIGN_KEY, "");
}
    
MetadataResourceList RetsMetadata::GetAllResources() const
{
    FinderHelper<MetadataResource> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::RESOURCE, "");
}

MetadataResource * RetsMetadata::GetResource(string resourceName) const
{
    string level = "";
    StringVector parameters;
    string resource = resourceName;
    
    ba::split(parameters, resourceName, ba::is_any_of(":"));
    StringVector::iterator i;
    for (i = parameters.begin(); i != parameters.end(); i++)
    {
        if (i == parameters.begin())
        {
            resource = ba::trim_copy(*i);
        }
        if (i != parameters.begin())
        {
            level = ba::trim_copy(*i);
        }
    }
    
    FinderHelper<MetadataResource> helper(mFinder);
    return helper.FindByPath(MetadataElement::RESOURCE, level, resource);
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

MetadataLookupTypeList RetsMetadata::GetAllLookupTypes(string resourceName,
                                                       string lookupName) const
{
    string level = resourceName + ":" + lookupName;
    FinderHelper<MetadataLookupType> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::LOOKUP_TYPE, level);
}


MetadataLookupTypeList RetsMetadata::GetAllLookupTypes(
    MetadataLookup * metadataLookup) const
{
    return GetAllLookupTypes(metadataLookup->GetLevel(),
                             metadataLookup->GetLookupName());
}


MetadataLookupType * RetsMetadata::GetLookupType(std::string resourceName,
                                                 std::string lookupName,
                                                 std::string lookupValue) const
{
    string level = resourceName + ":" + lookupName;
    FinderHelper<MetadataLookupType> helper(mFinder);
    return helper.FindByPath(MetadataElement::LOOKUP_TYPE, level, lookupValue);
}

MetadataObjectList RetsMetadata::GetAllObjects(std::string resourceName) const
{
    FinderHelper<MetadataObject> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::OBJECT, resourceName);
}

MetadataObjectList RetsMetadata::GetAllObjects(
    MetadataResource * metadataResource) const
{
    return GetAllObjects(metadataResource->GetResourceID());
}

MetadataSearchHelp * RetsMetadata::GetSearchHelp(
    std::string resourceName, std::string searchHelpID) const
{
    FinderHelper<MetadataSearchHelp> helper(mFinder);
    return helper.FindByPath(MetadataElement::SEARCH_HELP, resourceName,
                              searchHelpID);
}

MetadataColumnGroupList RetsMetadata::GetAllColumnGroups(MetadataClass * metadataClass) const
{
    return GetAllColumnGroups(metadataClass->GetLevel(), metadataClass->GetClassName());
}

MetadataColumnGroupList RetsMetadata::GetAllColumnGroups(std::string resourceName,
                                           std::string className) const
{
    string level = resourceName + ":" + className;
    FinderHelper<MetadataColumnGroup> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::COLUMN_GROUP, level);    
}

MetadataColumnGroupSetList RetsMetadata::GetAllColumnGroupSets(MetadataClass * metadataClass) const
{
    return GetAllColumnGroupSets(metadataClass->GetLevel(), metadataClass->GetClassName());
}

MetadataColumnGroupSetList RetsMetadata::GetAllColumnGroupSets(std::string resourceName,
                                              std::string className) const
{
    string level = resourceName + ":" + className;
    FinderHelper<MetadataColumnGroupSet> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::COLUMN_GROUP_SET, level);    
}

MetadataColumnGroupControlList RetsMetadata::GetAllColumnGroupControls(MetadataClass * metadataClass,
                                                         std::string columnGroup) const
{
    return GetAllColumnGroupControls(metadataClass->GetLevel(), metadataClass->GetClassName(), columnGroup);
}

MetadataColumnGroupControlList RetsMetadata::GetAllColumnGroupControls(std::string resourceName,
                                                         std::string className,
                                                         std::string columnGroup) const
{
    string level = resourceName + ":" + className + ":" + columnGroup;
    FinderHelper<MetadataColumnGroupControl> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::COLUMN_GROUP_CONTROL, level);    
}

MetadataColumnGroupTableList RetsMetadata::GetAllColumnGroupTables(MetadataClass * metadataClass,
                                                     std::string columnGroup) const
{
    return GetAllColumnGroupTables(metadataClass->GetLevel(), metadataClass->GetClassName(), columnGroup);
}

MetadataColumnGroupTableList RetsMetadata::GetAllColumnGroupTables(std::string resourceName,
                                                     std::string className,
                                                     std::string columnGroup) const
{
    string level = resourceName + ":" + className + ":" + columnGroup;
    FinderHelper<MetadataColumnGroupTable> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::COLUMN_GROUP_TABLE, level);    
}

MetadataColumnGroupNormalizationList RetsMetadata::GetAllColumnGroupNormalizations(MetadataClass * metadataClass,
                                                                    std::string columnGroup) const
{
    return GetAllColumnGroupNormalizations(metadataClass->GetLevel(), metadataClass->GetClassName(), columnGroup);
}

MetadataColumnGroupNormalizationList RetsMetadata::GetAllColumnGroupNormalizations(std::string resourceName,
                                                                    std::string className,
                                                                    std::string columnGroup) const
{
    string level = resourceName + ":" + className + ":" + columnGroup;
    FinderHelper<MetadataColumnGroupNormalization> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::COLUMN_GROUP_NORMALIZATION, level);    
}

MetadataFilterList RetsMetadata::GetAllFilters() const
{
    FinderHelper<MetadataFilter> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::FILTER, "");    
}

MetadataFilterTypeList RetsMetadata::GetAllFilterTypes(std::string filter) const
{
    FinderHelper<MetadataFilterType> helper(mFinder);
    return *helper.FindByLevel(MetadataElement::FILTER_TYPE, filter);    
}

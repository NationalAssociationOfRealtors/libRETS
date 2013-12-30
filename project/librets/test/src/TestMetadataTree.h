/*
 * Copyright (C) 2005-2009 National Association of REALTORS(R)
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

#ifndef LIBRETS_TEST_METADATA_TREE_H
#define LIBRETS_TEST_METADATA_TREE_H

#include "TestMetadataLoader.h"
#include "librets/RetsMetadata.h"
#include "librets/MetadataElementCollector.h"
#include "librets/MetadataElement.h"
#include "librets/MetadataForeignKey.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/MetadataLookup.h"
#include "librets/MetadataLookupType.h"
#include "librets/MetadataSearchHelp.h"
#include "librets/MetadataObject.h"
#include "librets/MetadataColumnGroup.h"
#include "librets/MetadataColumnGroupNormalization.h"
#include "librets/MetadataColumnGroupTable.h"
#include "librets/MetadataColumnGroupControl.h"
#include "librets/MetadataColumnGroupSet.h"
#include "librets/MetadataFilter.h"
#include "librets/MetadataFilterType.h"

namespace librets {
    
class TestMetadataTree
{
  public:
    TestMetadataTree(MetadataElementCollector * collector);
    
    MetadataForeignKeyList foreignKeys;
    MetadataResourceList resources;
    MetadataClassList classes;
    MetadataClassList propertyClasses;
    MetadataClassList agentClasses;
    MetadataTableList propertyResTables;
    MetadataLookupList propertyLookups;
    MetadataLookupTypeList propertyAreaLookupTypes;
    MetadataColumnGroupList                 columnGroups;
    MetadataColumnGroupSetList              columnGroupSets;
    MetadataColumnGroupControlList          columnGroupControls;
    MetadataColumnGroupTableList            columnGroupTables;
    MetadataColumnGroupNormalizationList    columnGroupNormalizations;
    MetadataFilterList                      filterList;
    MetadataFilterTypeList                  filterTypeList;
    MetadataSystemPtr system;
    
    MetadataForeignKeyPtr foreignKeyOne;
    MetadataForeignKeyPtr foreignKeyTwo;

    MetadataResourcePtr agentResource;
    MetadataResourcePtr propertyResource;

    MetadataClassPtr resClass;
    MetadataClassPtr lndClass;
    MetadataClassPtr agtClass;

    MetadataTablePtr listDateTable;
    MetadataTablePtr listPriceTable;
    
    MetadataLookupPtr areaLookup;

    MetadataLookupTypePtr southLoopLookupType;
    MetadataLookupTypePtr lincolnParkLookupType;

    MetadataObjectList objectClasses;
    MetadataObjectPtr propObject;

    MetadataSearchHelpPtr listPriceSearchHelp;
    
    MetadataColumnGroupPtr              columnGroup;
    MetadataColumnGroupNormalizationPtr columnGroupNormalization1;
    MetadataColumnGroupNormalizationPtr columnGroupNormalization2;
    MetadataColumnGroupNormalizationPtr columnGroupNormalization3;
    MetadataColumnGroupTablePtr         columnGroupTable1;
    MetadataColumnGroupTablePtr         columnGroupTable2;
    MetadataColumnGroupTablePtr         columnGroupTable3;
    MetadataColumnGroupControlPtr       columnGroupControl;
    MetadataColumnGroupSetPtr           columnGroupSet1;
    MetadataColumnGroupSetPtr           columnGroupSet2;
    MetadataFilterPtr                   filter;
    MetadataFilterTypePtr               filterType;
    
};

typedef boost::shared_ptr<TestMetadataTree> TestMetadataTreePtr;

}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

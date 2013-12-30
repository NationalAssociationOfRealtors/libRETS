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

#include "testUtil.h"
#include "TestMetadataTree.h"

using namespace librets;
using std::string;

#define CLASS TestMetadataTree

CLASS::CLASS(MetadataElementCollector * collector)
{
    system.reset(new MetadataSystem());
    system->SetLevel("");
    collector->AddElement(system);
    
    foreignKeyOne.reset(new MetadataForeignKey());
    foreignKeyOne->SetLevel("");
    foreignKeyOne->SetAttribute("ForeignKeyID", "Key1");
    collector->AddElement(foreignKeyOne);
    foreignKeys.push_back(foreignKeyOne.get());
    
    foreignKeyTwo.reset(new MetadataForeignKey());
    foreignKeyTwo->SetLevel("");
    foreignKeyTwo->SetAttribute("ForeignKeyID", "Key2");
    collector->AddElement(foreignKeyTwo);
    foreignKeys.push_back(foreignKeyTwo.get());
    
    propertyResource.reset(new MetadataResource());
    propertyResource->SetAttribute("ResourceID", "Property");
    propertyResource->SetLevel("");
    collector->AddElement(propertyResource);
    resources.push_back(propertyResource.get());
    
    agentResource.reset(new MetadataResource());
    agentResource->SetAttribute("ResourceID", "Agent");
    agentResource->SetLevel("");
    collector->AddElement(agentResource);
    resources.push_back(agentResource.get());
    
    resClass.reset(new MetadataClass());
    resClass->SetAttribute("ClassName", "RES");
    resClass->SetLevel("Property");
    collector->AddElement(resClass);
    classes.push_back(resClass.get());
    propertyClasses.push_back(resClass.get());
    
    lndClass.reset(new MetadataClass());
    lndClass->SetAttribute("ClassName", "LND");
    lndClass->SetLevel("Property");
    collector->AddElement(lndClass);
    classes.push_back(lndClass.get());
    propertyClasses.push_back(lndClass.get());
    
    agtClass.reset(new MetadataClass());
    agtClass->SetAttribute("ClassName", "AGT");
    agtClass->SetLevel("Agent");
    collector->AddElement(agtClass);
    classes.push_back(agtClass.get());
    agentClasses.push_back(agtClass.get());
    
    listPriceTable.reset(new MetadataTable());
    listPriceTable->SetAttribute("SystemName", "ListPrice");
    listPriceTable->SetLevel("Property:RES");
    listPriceTable->SetAttribute("SearchHelpID", "ListPrice");
    collector->AddElement(listPriceTable);
    propertyResTables.push_back(listPriceTable.get());
    
    listDateTable.reset(new MetadataTable());
    listDateTable->SetLevel("Property:RES");
    listDateTable->SetAttribute("SystemName", "ListDate");
    collector->AddElement(listDateTable);
    propertyResTables.push_back(listDateTable.get());
    
    areaLookup.reset(new MetadataLookup());
    areaLookup->SetLevel("Property");
    areaLookup->SetAttribute("LookupName", "AR");
    areaLookup->SetAttribute("VisibleName", "Area");
    collector->AddElement(areaLookup);
    propertyLookups.push_back(areaLookup.get());
    
    southLoopLookupType.reset(new MetadataLookupType());
    southLoopLookupType->SetLevel("Property:AR");
    southLoopLookupType->SetAttribute("Value", "SoLo");
    southLoopLookupType->SetAttribute("ShortValue", "SL");
    southLoopLookupType->SetAttribute("LongValue", "South Loop");
    collector->AddElement(southLoopLookupType);
    propertyAreaLookupTypes.push_back(southLoopLookupType.get());
    
    lincolnParkLookupType.reset(new MetadataLookupType());
    lincolnParkLookupType->SetLevel("Property:AR");
    lincolnParkLookupType->SetAttribute("Value", "LP");
    lincolnParkLookupType->SetAttribute("ShortValue", "LP");
    lincolnParkLookupType->SetAttribute("LongValue", "Linconln Park");
    collector->AddElement(lincolnParkLookupType);
    propertyAreaLookupTypes.push_back(lincolnParkLookupType.get());

    listPriceSearchHelp.reset(new MetadataSearchHelp());
    listPriceSearchHelp->SetLevel("Property");
    listPriceSearchHelp->SetAttribute("SearchHelpID", "ListPrice");
    listPriceSearchHelp->SetAttribute("Value", "This tells you the price of "
                                      "the property, DUH");
    collector->AddElement(listPriceSearchHelp);

    propObject.reset(new MetadataObject());
    propObject->SetLevel("Property");
    propObject->SetAttribute("ObjectType", "Photo");
    propObject->SetAttribute("MIMEType", "foo");
    propObject->SetAttribute("VisibleName", "foo");
    propObject->SetAttribute("Description", "foo");
    collector->AddElement(propObject);
    objectClasses.push_back(propObject.get());
    
    columnGroup.reset(new MetadataColumnGroup());
    columnGroup->SetLevel("Property:RES");
    columnGroup->SetAttribute("Version", "1.00.000");
    columnGroup->SetAttribute("Date", "Thu, 3 Feb 2005 20:35:15 GMT");
    columnGroup->SetAttribute("MetadataEntryID", "10001123456");
    columnGroup->SetAttribute("ColumnGroupName", "WaterFront");
    columnGroup->SetAttribute("ControlSystemName", "WaterFrontFlag");
    columnGroup->SetAttribute("LongName", "Water Front Information");
    columnGroup->SetAttribute("ShortName", "Water Front");
    columnGroup->SetAttribute("Description", "Listing data that contains water front information for the Listing");
    collector->AddElement(columnGroup);
    columnGroups.push_back(columnGroup.get());
    
    columnGroupSet1.reset(new MetadataColumnGroupSet());
    columnGroupSet1->SetLevel("Property:RES");
    columnGroupSet1->SetAttribute("Version", "1.00.000");
    columnGroupSet1->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupSet1->SetAttribute("MetadataEntryID", "100000123456");
    columnGroupSet1->SetAttribute("ColumnGroupSetName", "Residential");
    columnGroupSet1->SetAttribute("ColumnGroupSetParent", "");
    columnGroupSet1->SetAttribute("Sequence", "1");
    columnGroupSet1->SetAttribute("LongName", "Residential Listing");
    columnGroupSet1->SetAttribute("ShortName", "Residential");
    columnGroupSet1->SetAttribute("Description", "The top node of the Residential Listing Data Entry Hierarchy");
    columnGroupSet1->SetAttribute("ColumnGroupName", "");
    columnGroupSet1->SetAttribute("PresentationStyle", "");
    columnGroupSet1->SetAttribute("URL", "");
    columnGroupSet1->SetAttribute("ForeighKeyID", "");
    collector->AddElement(columnGroupSet1);
    columnGroupSets.push_back(columnGroupSet1.get());
    
    columnGroupSet2.reset(new MetadataColumnGroupSet());
    columnGroupSet2->SetLevel("Property:RES");
    columnGroupSet2->SetAttribute("Version", "1.00.000");
    columnGroupSet2->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupSet2->SetAttribute("MetadataEntryID", "10000123457");
    columnGroupSet2->SetAttribute("ColumnGroupSetName", "WaterFront");
    columnGroupSet2->SetAttribute("ColumnGroupSetParent", "Residential");
    columnGroupSet2->SetAttribute("Sequence", "1");
    columnGroupSet2->SetAttribute("LongName", "WaterFront Information");
    columnGroupSet2->SetAttribute("ShortName", "WaterFront");
    columnGroupSet2->SetAttribute("Description", "Details about water front for property");
    columnGroupSet2->SetAttribute("ColumnGroupName", "WaterFront");
    columnGroupSet2->SetAttribute("PresentationStyle", "Edit1");
    columnGroupSet2->SetAttribute("URL", "");
    columnGroupSet2->SetAttribute("ForeighKeyID", "");
    collector->AddElement(columnGroupSet2);
    columnGroupSets.push_back(columnGroupSet2.get());
    
    columnGroupControl.reset(new MetadataColumnGroupControl());
    columnGroupControl->SetLevel("Property:RES:WaterFront");
    columnGroupControl->SetAttribute("Version", "1.00.000");
    columnGroupControl->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupControl->SetAttribute("MetadataEntryID", "10011123456");
    columnGroupControl->SetAttribute("LowValue", "1");
    columnGroupControl->SetAttribute("HighValue", "1");
    collector->AddElement(columnGroupControl);
    columnGroupControls.push_back(columnGroupControl.get());
    
    columnGroupTable1.reset(new MetadataColumnGroupTable());
    columnGroupTable1->SetLevel("Property:RES:WaterFront");
    columnGroupTable1->SetAttribute("Version", "1.00.000");
    columnGroupTable1->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupTable1->SetAttribute("MetadataEntryID", "10111123450");
    columnGroupTable1->SetAttribute("SystemName", "WaterFront");
    columnGroupTable1->SetAttribute("DisplayOrder", "1");
    collector->AddElement(columnGroupTable1);
    columnGroupTables.push_back(columnGroupTable1.get());
    
    columnGroupTable2.reset(new MetadataColumnGroupTable());
    columnGroupTable2->SetLevel("Property:RES:WaterFront");
    columnGroupTable2->SetAttribute("Version", "1.00.000");
    columnGroupTable2->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupTable2->SetAttribute("MetadataEntryID", "10111123451");
    columnGroupTable2->SetAttribute("SystemName", "WaterAccess");
    columnGroupTable2->SetAttribute("DisplayOrder", "2");
    collector->AddElement(columnGroupTable2);
    columnGroupTables.push_back(columnGroupTable2.get());
    
    columnGroupTable3.reset(new MetadataColumnGroupTable());
    columnGroupTable3->SetLevel("Property:RES:WaterFront");
    columnGroupTable3->SetAttribute("Version", "1.00.000");
    columnGroupTable3->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupTable3->SetAttribute("MetadataEntryID", "10111123452");
    columnGroupTable3->SetAttribute("SystemName", "WaterFrontage");
    columnGroupTable3->SetAttribute("DisplayOrder", "3");
    collector->AddElement(columnGroupTable3);
    columnGroupTables.push_back(columnGroupTable3.get());
    
    columnGroupNormalization1.reset(new MetadataColumnGroupNormalization());
    columnGroupNormalization1->SetLevel("Property:RES:WaterFront");
    columnGroupNormalization1->SetAttribute("Version", "1.00.000");
    columnGroupNormalization1->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupNormalization1->SetAttribute("MetadataEntryID", "10211123450");
    columnGroupNormalization1->SetAttribute("TypeIdentifier", "Bedroom");
    columnGroupNormalization1->SetAttribute("Sequence", "1");
    columnGroupNormalization1->SetAttribute("ColumnLabel", "Length");
    columnGroupNormalization1->SetAttribute("SystemName", "Bedroom1Length");
    collector->AddElement(columnGroupNormalization1);
    columnGroupNormalizations.push_back(columnGroupNormalization1.get());
    
    columnGroupNormalization2.reset(new MetadataColumnGroupNormalization());
    columnGroupNormalization2->SetLevel("Property:RES:WaterFront");
    columnGroupNormalization2->SetAttribute("Version", "1.00.000");
    columnGroupNormalization2->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupNormalization2->SetAttribute("MetadataEntryID", "10211123451");
    columnGroupNormalization2->SetAttribute("TypeIdentifier", "Bedroom");
    columnGroupNormalization2->SetAttribute("Sequence", "1");
    columnGroupNormalization2->SetAttribute("ColumnLabel", "Wodtj");
    columnGroupNormalization2->SetAttribute("SystemName", "Bedroom1Width");
    collector->AddElement(columnGroupNormalization2);
    columnGroupNormalizations.push_back(columnGroupNormalization2.get());
    
    columnGroupNormalization3.reset(new MetadataColumnGroupNormalization());
    columnGroupNormalization3->SetLevel("Property:RES:WaterFront");
    columnGroupNormalization3->SetAttribute("Version", "1.00.000");
    columnGroupNormalization3->SetAttribute("Date", "Sat, 20 Mar 2002 12:03:38 GMT");
    columnGroupNormalization3->SetAttribute("MetadataEntryID", "10211123452");
    columnGroupNormalization3->SetAttribute("TypeIdentifier", "Bedroom");
    columnGroupNormalization3->SetAttribute("Sequence", "1");
    columnGroupNormalization3->SetAttribute("ColumnLabel", "Area");
    columnGroupNormalization3->SetAttribute("SystemName", "Bedroom1Area");
    collector->AddElement(columnGroupNormalization3);
    columnGroupNormalizations.push_back(columnGroupNormalization3.get());
    
    filter.reset(new MetadataFilter());
    filter->SetLevel("");
    filter->SetAttribute("FilterID", "foo");
    filter->SetAttribute("ParentResource", "parent resource");
    filter->SetAttribute("ParentLookupName", "parent lookup");
    filter->SetAttribute("ChildResource", "child resource");
    filter->SetAttribute("ChildLookupName", "child lookup");
    filter->SetAttribute("NotShownnByDefault", "1");

    filterType.reset(new MetadataFilterType());
    filterType->SetLevel("foo");
    filterType->SetAttribute("ParentValue", "parent value");
    filterType->SetAttribute("ChildValue", "child value");
}

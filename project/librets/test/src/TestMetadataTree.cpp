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
#include "TestMetadataTree.h"

using namespace librets;
using std::string;

#define CLASS TestMetadataTree

CLASS::CLASS(MetadataElementCollector * collector)
{
    system.reset(new MetadataSystem());
    system->SetLevel("");
    collector->AddElement(system);
    
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

    propObject.reset(new MetadataObject());
    propObject->SetLevel("Property");
    propObject->SetAttribute("ObjectType", "Photo");
    propObject->SetAttribute("MIMEType", "foo");
    propObject->SetAttribute("VisibleName", "foo");
    propObject->SetAttribute("Description", "foo");
    collector->AddElement(propObject);
    objectClasses.push_back(propObject.get());
}

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
#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "librets/RetsMetadata.h"
#include "librets/MetadataByLevelCollector.h"
#include "librets/MetadataElement.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"

using namespace librets;
using namespace std;

#define CLASS RetsMetadataTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testGetSystem);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testGetSystem();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testGetSystem()
{
    MetadataByLevelCollectorPtr collector(new MetadataByLevelCollector());
    MetadataSystemPtr system(new MetadataSystem());
    system->SetLevel("");
    collector->AddElement(system);

    MetadataResourceList resources;
    MetadataResourcePtr resource(new MetadataResource());
    resource->SetLevel("");
    resource->SetAttribute("ResourceID", "Property");
    collector->AddElement(resource);
    resources.push_back(resource.get());
    resource.reset(new MetadataResource());
    resource->SetLevel("");
    resource->SetAttribute("ResourceID", "Agent");
    collector->AddElement(resource);
    resources.push_back(resource.get());
    MetadataResource * agentResource = resource.get();

    MetadataClassList classes;
    MetadataClassList propClasses;
    MetadataClassPtr resClass(new MetadataClass());
    resClass->SetLevel("Property");
    resClass->SetAttribute("ClassName", "RES");
    collector->AddElement(resClass);
    classes.push_back(resClass.get());
    propClasses.push_back(resClass.get());
    MetadataClassPtr aClass(new MetadataClass());
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Property");
    aClass->SetAttribute("ClassName", "LND");
    collector->AddElement(aClass);
    classes.push_back(aClass.get());
    propClasses.push_back(aClass.get());

    MetadataClassList agentClasses;
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Agent");
    aClass->SetAttribute("ClassName", "AGT");
    collector->AddElement(aClass);
    classes.push_back(aClass.get());
    agentClasses.push_back(aClass.get());

    MetadataTableList tables;
    MetadataTablePtr table(new MetadataTable());
    table->SetLevel("Property:RES");
    table->SetAttribute("SystemName", "LP");
    collector->AddElement(table);
    tables.push_back(table.get());
    table.reset(new MetadataTable());
    table->SetLevel("Property:RES");
    table->SetAttribute("SystemName", "LD");
    collector->AddElement(table);
    tables.push_back(table.get());
    MetadataTable * ldTable = table.get();

    RetsMetadataPtr metadata(new RetsMetadata(collector));

    MetadataSystem * actualSystem = metadata->GetSystem();
    CPPUNIT_ASSERT(actualSystem);

    MetadataResourceList actualResources = metadata->GetAllResources();
    ASSERT_VECTOR_EQUAL(resources, actualResources);

    MetadataResource * actualResource = metadata->GetResource("Agent");
    ASSERT_EQUAL(agentResource, actualResource);

    MetadataClassList actualClasses = metadata->GetAllClasses();
    ASSERT_VECTOR_EQUAL(classes, actualClasses);

    MetadataClass * actualClass = metadata->GetClass("Property", "RES");
    ASSERT_EQUAL(*resClass, *actualClass);

    MetadataTableList actualTables =
        metadata->GetTablesForClass(actualClasses.at(0));
    ASSERT_VECTOR_EQUAL(tables, actualTables);
    
    actualTables = metadata->GetTablesForClass("Property", "RES");
    ASSERT_VECTOR_EQUAL(tables, actualTables);

    MetadataTable * actualTable = metadata->GetTable("Property", "RES", "LD");
    ASSERT_EQUAL(ldTable, actualTable);

    MetadataClassList actualPropClasses =
        metadata->GetClassesForResource("Property");
    ASSERT_VECTOR_EQUAL(propClasses, actualPropClasses);

    MetadataClassList actualAgentClasses =
        metadata->GetClassesForResource("Agent");
    ASSERT_VECTOR_EQUAL(agentClasses, actualAgentClasses);
}

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
using namespace librets::util;
using namespace std;

#define CLASS RetsMetadataTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testGetSystem);
    CPPUNIT_TEST(testGetAllResources);
    CPPUNIT_TEST(testGetResource);
    CPPUNIT_TEST(testGetClass);
    CPPUNIT_TEST(testGetTablesByClassObject);
    CPPUNIT_TEST(testGetTablesByClassName);
    CPPUNIT_TEST(testGetClassesForResource);
    CPPUNIT_TEST(testGetTable);
    CPPUNIT_TEST_SUITE_END();
    
  protected:
    void testGetSystem();
    void testGetAllResources();
    void testGetResource();
    void testGetClass();
    void testGetTablesByClassObject();
    void testGetTablesByClassName();
    void testGetClassesForResource();
    void testGetTable();
    
  public:
    void setUp();
    void tearDown();
    
  private:
    MetadataByLevelCollectorPtr collector;
    MetadataResourceList resources;
    MetadataClassList classes;
    MetadataClassList propClasses;
    MetadataClassList agentClasses;
    MetadataTableList tables;
    
    MetadataResource * agentResource;
    MetadataClass * resClass;
    MetadataTable * ldTable;
    RetsMetadataPtr metadata;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::setUp()
{
    MetadataByLevelCollectorPtr collector(new MetadataByLevelCollector());
    MetadataSystemPtr system(new MetadataSystem());
    system->SetLevel("");
    collector->AddElement(system);
    
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
    agentResource = resource.get();
    
    MetadataClassPtr aClass(new MetadataClass());
    aClass->SetLevel("Property");
    aClass->SetAttribute("ClassName", "RES");
    collector->AddElement(aClass);
    classes.push_back(aClass.get());
    propClasses.push_back(aClass.get());
    resClass = aClass.get();

    aClass.reset(new MetadataClass());
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Property");
    aClass->SetAttribute("ClassName", "LND");
    collector->AddElement(aClass);
    classes.push_back(aClass.get());
    propClasses.push_back(aClass.get());
    
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Agent");
    aClass->SetAttribute("ClassName", "AGT");
    collector->AddElement(aClass);
    classes.push_back(aClass.get());
    agentClasses.push_back(aClass.get());
    
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
    ldTable = table.get();
    
    metadata.reset(new RetsMetadata(collector));
}

void CLASS::tearDown()
{
    collector.reset();
    resources.clear();
    classes.clear();
    agentClasses.clear();
    propClasses.clear();
    tables.clear();
    
    agentResource = 0;
    resClass =  0;
    ldTable = 0;

    metadata.reset();
}

void CLASS::testGetSystem()
{
    MetadataSystem * actualSystem = metadata->GetSystem();
    CPPUNIT_ASSERT(actualSystem);
}

void CLASS::testGetAllResources()
{
    MetadataResourceList actualResources = metadata->GetAllResources();
    ASSERT_VECTOR_EQUAL(resources, actualResources);
}

void CLASS::testGetResource()
{
    MetadataResource * actualResource = metadata->GetResource("Agent");
    ASSERT_EQUAL(agentResource, actualResource);
}

void CLASS::testGetClass()
{
    MetadataClass * actualClass = metadata->GetClass("Property", "RES");
    ASSERT_EQUAL(*resClass, *actualClass);
}

void CLASS::testGetTablesByClassObject()
{
    MetadataClass * actualClass = metadata->GetClass("Property", "RES");
    MetadataTableList actualTables =
        metadata->GetTablesForClass(actualClass);
    ASSERT_VECTOR_EQUAL(tables, actualTables);
}

void CLASS::testGetTablesByClassName()
{
    MetadataTableList actualTables =
        metadata->GetTablesForClass("Property", "RES");
    ASSERT_VECTOR_EQUAL(tables, actualTables);
}

void CLASS::testGetClassesForResource()
{
    MetadataClassList actualPropClasses =
    metadata->GetClassesForResource("Property");
    ASSERT_VECTOR_EQUAL(propClasses, actualPropClasses);
    
    MetadataClassList actualAgentClasses =
        metadata->GetClassesForResource("Agent");
    ASSERT_VECTOR_EQUAL(agentClasses, actualAgentClasses);
}

void CLASS::testGetTable()
{
    MetadataTable * actualTable = metadata->GetTable("Property", "RES", "LD");
    ASSERT_EQUAL(ldTable, actualTable);
}

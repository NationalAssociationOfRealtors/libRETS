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
#include "TestMetadataLoader.h"
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

#define CLASS_ RetsMetadataTest

class CLASS_ : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS_);
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
    TestMetadataLoaderPtr loader;
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

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS_);

void CLASS_::setUp()
{
    loader.reset(new TestMetadataLoader());
    collector.reset(new MetadataByLevelCollector());
    
    MetadataSystemPtr system(new MetadataSystem());
    system->SetLevel("");
    loader->AddElement(system);
    
    MetadataResourcePtr resource(new MetadataResource());
    resource->SetLevel("");
    resource->SetAttribute("ResourceID", "Property");
    loader->AddElement(resource);
    resources.push_back(resource.get());
    resource.reset(new MetadataResource());
    resource->SetLevel("");
    resource->SetAttribute("ResourceID", "Agent");
    loader->AddElement(resource);
    resources.push_back(resource.get());
    agentResource = resource.get();
    
    MetadataClassPtr aClass(new MetadataClass());
    aClass->SetLevel("Property");
    aClass->SetAttribute("ClassName", "RES");
    loader->AddElement(aClass);
    classes.push_back(aClass.get());
    propClasses.push_back(aClass.get());
    resClass = aClass.get();

    aClass.reset(new MetadataClass());
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Property");
    aClass->SetAttribute("ClassName", "LND");
    loader->AddElement(aClass);
    classes.push_back(aClass.get());
    propClasses.push_back(aClass.get());
    
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Agent");
    aClass->SetAttribute("ClassName", "AGT");
    loader->AddElement(aClass);
    classes.push_back(aClass.get());
    agentClasses.push_back(aClass.get());
    
    MetadataTablePtr table(new MetadataTable());
    table->SetLevel("Property:RES");
    table->SetAttribute("SystemName", "LP");
    loader->AddElement(table);
    tables.push_back(table.get());
    table.reset(new MetadataTable());
    table->SetLevel("Property:RES");
    table->SetAttribute("SystemName", "LD");
    loader->AddElement(table);
    tables.push_back(table.get());
    ldTable = table.get();
    
    metadata.reset(new RetsMetadata(collector, loader.get()));
}

void CLASS_::tearDown()
{
    loader.reset();
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

void CLASS_::testGetSystem()
{
    MetadataSystem * actualSystem = metadata->GetSystem();
    CPPUNIT_ASSERT(actualSystem);
    ASSERT_EQUAL(1, loader->GetTotalLoadCount());
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::SYSTEM));
}

void CLASS_::testGetAllResources()
{
    MetadataResourceList actualResources = metadata->GetAllResources();
    ASSERT_VECTOR_EQUAL(resources, actualResources);
    ASSERT_EQUAL(2, loader->GetTotalLoadCount());
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::RESOURCE));
}

void CLASS_::testGetResource()
{
    MetadataResource * actualResource = metadata->GetResource("Agent");
    ASSERT_EQUAL(agentResource, actualResource);
    ASSERT_EQUAL(2, loader->GetTotalLoadCount());
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::RESOURCE));
}

void CLASS_::testGetClass()
{
    MetadataClass * actualClass = metadata->GetClass("Property", "RES");
    CPPUNIT_ASSERT(actualClass);
    ASSERT_EQUAL(*resClass, *actualClass);
    ASSERT_EQUAL(2, loader->GetTotalLoadCount());
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::CLASS));
}

void CLASS_::testGetClassesForResource()
{
    MetadataClassList actualPropClasses =
        metadata->GetClassesForResource("Property");
    ASSERT_VECTOR_EQUAL(propClasses, actualPropClasses);
    
    MetadataClassList actualAgentClasses =
        metadata->GetClassesForResource("Agent");
    ASSERT_VECTOR_EQUAL(agentClasses, actualAgentClasses);
    ASSERT_EQUAL(3, loader->GetTotalLoadCount());
    ASSERT_EQUAL(2, loader->GetLoadCount(MetadataElement::CLASS));
}

void CLASS_::testGetTablesByClassObject()
{
    MetadataClass * actualClass = metadata->GetClass("Property", "RES");
    CPPUNIT_ASSERT(actualClass);
    MetadataTableList actualTables =
        metadata->GetTablesForClass(actualClass);
    ASSERT_VECTOR_EQUAL(tables, actualTables);
    ASSERT_EQUAL(3, loader->GetTotalLoadCount());
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::CLASS));
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::TABLE));
}

void CLASS_::testGetTablesByClassName()
{
    MetadataTableList actualTables =
        metadata->GetTablesForClass("Property", "RES");
    ASSERT_VECTOR_EQUAL(tables, actualTables);
    ASSERT_EQUAL(2, loader->GetTotalLoadCount());
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::TABLE));
}

void CLASS_::testGetTable()
{
    MetadataTable * actualTable = metadata->GetTable("Property", "RES", "LD");
    CPPUNIT_ASSERT(actualTable);
    ASSERT_EQUAL(*ldTable, *actualTable);
    ASSERT_EQUAL(2, loader->GetTotalLoadCount());
    ASSERT_EQUAL(1, loader->GetLoadCount(MetadataElement::TABLE));
}

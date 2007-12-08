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
#include "librets/DefaultMetadataCollector.h"
#include "TestMetadataTree.h"

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
    CPPUNIT_TEST(testGetAllClasses);
    CPPUNIT_TEST(testGetClass);
    CPPUNIT_TEST(testGetAllTablesByClassObject);
    CPPUNIT_TEST(testGetAllTablesByClassName);
    CPPUNIT_TEST(testGetTable);
    CPPUNIT_TEST(testGetAllLookups);
    CPPUNIT_TEST(testGetLookup);
    CPPUNIT_TEST(testGetAllLookupTypesByName);
    CPPUNIT_TEST(testGetAllLookupTypesByObject);
    CPPUNIT_TEST(testGetLookupType);
    CPPUNIT_TEST(testGetAllObjectsByName);
    CPPUNIT_TEST(testGetAllObjectsByObject);
    CPPUNIT_TEST(testGetSearchHelp);
    CPPUNIT_TEST_SUITE_END();
    
  protected:
    void testGetSystem();
    void testGetAllResources();
    void testGetResource();
    void testGetAllClasses();
    void testGetClass();
    void testGetAllTablesByClassObject();
    void testGetAllTablesByClassName();
    void testGetTable();
    void testGetAllLookups();
    void testGetLookup();
    void testGetAllLookupTypesByName();
    void testGetAllLookupTypesByObject();
    void testGetLookupType();
    void testGetAllObjectsByName();
    void testGetAllObjectsByObject();
    void testGetSearchHelp();
    
  public:
    void setUp();
    
  private:
    DefaultMetadataCollectorPtr mCollector;
    TestMetadataTreePtr mMetadataTree;
    RetsMetadataPtr metadata;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS_);

void CLASS_::setUp()
{
    mCollector.reset(new DefaultMetadataCollector());
    mMetadataTree.reset(new TestMetadataTree(mCollector.get()));
    metadata.reset(new RetsMetadata(mCollector));
}

void CLASS_::testGetSystem()
{
    MetadataSystem * actualSystem = metadata->GetSystem();
    CPPUNIT_ASSERT(actualSystem);
    ASSERT_EQUAL(*mMetadataTree->system, *actualSystem);
}

void CLASS_::testGetAllResources()
{
    MetadataResourceList actualResources = metadata->GetAllResources();
    ASSERT_VECTOR_EQUAL(mMetadataTree->resources, actualResources);
}

void CLASS_::testGetResource()
{
    MetadataResource * actualResource = metadata->GetResource("Agent");
    CPPUNIT_ASSERT(actualResource);
    ASSERT_EQUAL(*mMetadataTree->agentResource, *actualResource);
}

void CLASS_::testGetAllClasses()
{
    MetadataClassList actualPropClasses =
    metadata->GetAllClasses("Property");
    ASSERT_VECTOR_EQUAL(mMetadataTree->propertyClasses, actualPropClasses);
    
    MetadataClassList actualAgentClasses =
        metadata->GetAllClasses("Agent");
    ASSERT_VECTOR_EQUAL(mMetadataTree->agentClasses, actualAgentClasses);
}

void CLASS_::testGetClass()
{
    MetadataClass * actualClass = metadata->GetClass("Property", "RES");
    CPPUNIT_ASSERT(actualClass);
    ASSERT_EQUAL(*mMetadataTree->resClass, *actualClass);
}

void CLASS_::testGetAllTablesByClassObject()
{
    MetadataClass * actualClass = metadata->GetClass("Property", "RES");
    CPPUNIT_ASSERT(actualClass);
    MetadataTableList actualTables =
        metadata->GetAllTables(actualClass);
    ASSERT_VECTOR_EQUAL(mMetadataTree->propertyResTables, actualTables);
}

void CLASS_::testGetAllTablesByClassName()
{
    MetadataTableList actualTables =
        metadata->GetAllTables("Property", "RES");
    ASSERT_VECTOR_EQUAL(mMetadataTree->propertyResTables, actualTables);
}

void CLASS_::testGetTable()
{
    MetadataTable * actualTable =
        metadata->GetTable("Property", "RES", "ListDate");
    CPPUNIT_ASSERT(actualTable);
    ASSERT_EQUAL(*mMetadataTree->listDateTable, *actualTable);
}

void CLASS_::testGetAllLookups()
{
    MetadataLookupList actualLookups = metadata->GetAllLookups("Property");
    ASSERT_VECTOR_EQUAL(mMetadataTree->propertyLookups, actualLookups);
}

void CLASS_::testGetLookup()
{
    MetadataLookup * actualLookup = metadata->GetLookup("Property", "AR");
    CPPUNIT_ASSERT(actualLookup);
    ASSERT_EQUAL(*mMetadataTree->areaLookup, *actualLookup);
}

void CLASS_::testGetAllLookupTypesByName()
{
    MetadataLookupTypeList actualLookupTypes =
        metadata->GetAllLookupTypes("Property", "AR");
    ASSERT_VECTOR_EQUAL(mMetadataTree->propertyAreaLookupTypes,
                        actualLookupTypes);
}

void CLASS_::testGetAllLookupTypesByObject()
{
    MetadataLookup * actualLookup = metadata->GetLookup("Property", "AR");
    CPPUNIT_ASSERT(actualLookup);
    MetadataLookupTypeList actualLookupTypes =
        metadata->GetAllLookupTypes(actualLookup);
    ASSERT_VECTOR_EQUAL(mMetadataTree->propertyAreaLookupTypes,
                        actualLookupTypes);
}

void CLASS_::testGetLookupType()
{
    MetadataLookupType * actualLookupType =
        metadata->GetLookupType("Property", "AR", "SoLo");
    CPPUNIT_ASSERT(actualLookupType);
    ASSERT_EQUAL(*mMetadataTree->southLoopLookupType, *actualLookupType);
}

void CLASS_::testGetAllObjectsByName()
{
    MetadataObjectList actualObjectClasses =
        metadata->GetAllObjects("Property");
    ASSERT_VECTOR_EQUAL(mMetadataTree->objectClasses, actualObjectClasses);
}

void CLASS_::testGetAllObjectsByObject()
{
    MetadataResource * actualResource = metadata->GetResource("Property");
    CPPUNIT_ASSERT(actualResource);
    MetadataObjectList actualObjectClasses =
        metadata->GetAllObjects(actualResource);
    ASSERT_VECTOR_EQUAL(mMetadataTree->objectClasses, actualObjectClasses);
    
}

void CLASS_::testGetSearchHelp()
{
    MetadataSearchHelp* actualHelp = metadata->GetSearchHelp("Property",
                                                             "ListPrice");
    CPPUNIT_ASSERT(actualHelp);
    ASSERT_EQUAL(*mMetadataTree->listPriceSearchHelp, *actualHelp);
}

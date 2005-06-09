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

    MetadataResourceListPtr resources(new MetadataResourceList());
    MetadataResourcePtr resource(new MetadataResource());
    resource->SetLevel("");
    resource->SetAttribute("ResourceID", "Property");
    collector->AddElement(resource);
    resources->push_back(resource);
    resource.reset(new MetadataResource());
    resource->SetLevel("");
    resource->SetAttribute("ResourceID", "Agent");
    collector->AddElement(resource);
    resources->push_back(resource);
    MetadataResourcePtr agentResource = resource;

    MetadataClassListPtr classes(new MetadataClassList());
    MetadataClassListPtr propClasses(new MetadataClassList());
    MetadataClassPtr resClass(new MetadataClass());
    resClass->SetLevel("Property");
    resClass->SetAttribute("ClassName", "RES");
    collector->AddElement(resClass);
    classes->push_back(resClass);
    propClasses->push_back(resClass);
    MetadataClassPtr aClass(new MetadataClass());
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Property");
    aClass->SetAttribute("ClassName", "LND");
    collector->AddElement(aClass);
    classes->push_back(aClass);
    propClasses->push_back(aClass);

    MetadataClassListPtr agentClasses(new MetadataClassList());
    aClass.reset(new MetadataClass());
    aClass->SetLevel("Agent");
    aClass->SetAttribute("ClassName", "AGT");
    collector->AddElement(aClass);
    classes->push_back(aClass);
    agentClasses->push_back(aClass);

    MetadataTableListPtr tables(new MetadataTableList());
    MetadataTablePtr table(new MetadataTable());
    table->SetLevel("Property:RES");
    table->SetAttribute("SystemName", "LP");
    collector->AddElement(table);
    tables->push_back(table);
    table.reset(new MetadataTable());
    table->SetLevel("Property:RES");
    table->SetAttribute("SystemName", "LD");
    collector->AddElement(table);
    tables->push_back(table);
    MetadataTablePtr ldTable = table;

    RetsMetadataPtr metadata(new RetsMetadata(collector));

    MetadataSystemPtr actualSystem = metadata->GetSystem();
    CPPUNIT_ASSERT(actualSystem);

    MetadataResourceListPtr actualResources = metadata->GetAllResources();
    ASSERT_VECTOR_EQUAL(*resources, *actualResources);

    MetadataResourcePtr actualResource = metadata->GetResource("Agent");
    ASSERT_EQUAL(agentResource, actualResource);

    MetadataClassListPtr actualClasses = metadata->GetAllClasses();
    ASSERT_VECTOR_EQUAL(*classes, *actualClasses);

    MetadataClassPtr actualClass = metadata->GetClass("Property", "RES");
    ASSERT_EQUAL(resClass, actualClass);

    MetadataTableListPtr actualTables =
        metadata->GetTablesForClass(actualClasses->at(0));
    ASSERT_VECTOR_EQUAL(*tables, *actualTables);
    
    actualTables = metadata->GetTablesForClass("Property", "RES");
    ASSERT_VECTOR_EQUAL(*tables, *actualTables);

    MetadataTablePtr actualTable = metadata->GetTable("Property", "RES", "LD");
    ASSERT_EQUAL(ldTable, actualTable);

    MetadataClassListPtr actualPropClasses =
        metadata->GetClassesForResource("Property");
    ASSERT_VECTOR_EQUAL(*propClasses, *actualPropClasses);

    MetadataClassListPtr actualAgentClasses =
        metadata->GetClassesForResource("Agent");
    ASSERT_VECTOR_EQUAL(*agentClasses, *actualAgentClasses);
}

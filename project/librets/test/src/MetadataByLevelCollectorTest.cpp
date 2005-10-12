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
#include "librets/MetadataByLevelCollector.h"
#include "librets/MetadataElement.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataTable.h"

using namespace librets;
using namespace std;

#define CLASS MetadataByLevelCollectorTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testFindByLevel);
    CPPUNIT_TEST(testFindByPath);
    CPPUNIT_TEST_SUITE_END();
    
  public:
    void setUp();

  protected:
    void testFindByLevel();
    void testFindByPath();
    
    MetadataByLevelCollectorPtr mCollector;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::setUp()
{
    mCollector.reset(new MetadataByLevelCollector());
    MetadataElementPtr element(new MetadataSystem());
    element->SetLevel("");
    mCollector->AddElement(element);
    
    element.reset(new MetadataTable());
    element->SetLevel("Property:RES");
    element->SetAttribute("SystemName", "ListPrice");
    mCollector->AddElement(element);
    
    element.reset(new MetadataTable());
    element->SetLevel("Property:RES");
    element->SetAttribute("SystemName", "Beds");
    mCollector->AddElement(element);
}

void CLASS::testFindByLevel()
{
    MetadataElementListPtr elements;
    MetadataElementPtr element;
    
    elements = mCollector->FindByLevel(MetadataElement::SYSTEM, "");
    ASSERT_EQUAL(size_t(1), elements->size());
    element = elements->at(0);
    ASSERT_EQUAL(MetadataElement::SYSTEM, element->GetType());

    elements = mCollector->FindByLevel(MetadataElement::TABLE, "Property:RES");
    ASSERT_EQUAL(size_t(2), elements->size());
    element = elements->at(0);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("ListPrice", element->GetId());
    element = elements->at(1);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("Beds", element->GetId());

    elements = mCollector->FindByLevel(MetadataElement::TABLE, "Property:CON");
    ASSERT_EQUAL(size_t(0), elements->size());

    elements = mCollector->FindByLevel(MetadataElement::UPDATE, "Property:RES");
    ASSERT_EQUAL(size_t(0), elements->size());
}


void CLASS::testFindByPath()
{
    MetadataElementPtr element;
    element = mCollector->FindByPath(MetadataElement::TABLE,
                                     "Property:RES:ListPrice");
    CPPUNIT_ASSERT(element);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("ListPrice", element->GetId());
    
    element = mCollector->FindByPath(MetadataElement::TABLE,
                                     "Property:RES:Beds");
    CPPUNIT_ASSERT(element);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("Beds", element->GetId());

    element = mCollector->FindByPath(MetadataElement::TABLE,
                                     "Property:CON:Beds");
    CPPUNIT_ASSERT(!element);
}

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
#include "TestMetadataTree.h"
#include "librets/IncrementalMetadataFinder.h"

using namespace librets;
using namespace std;

#define CLASS IncrementalMetadataFinderTest

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
    
    TestMetadataLoaderPtr mLoader;
    TestMetadataTreePtr mMetadataTree;
    IncrementalMetadataFinderPtr mFinder;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::setUp()
{
    mLoader.reset(new TestMetadataLoader());
    mMetadataTree.reset(new TestMetadataTree(mLoader.get()));
    mFinder.reset(new IncrementalMetadataFinder(mLoader.get()));
}

void CLASS::testFindByLevel()
{
    ASSERT_EQUAL(0, mLoader->GetTotalLoadCount());
    
    MetadataElementListPtr elements;
    MetadataElementPtr element;
    
    elements = mFinder->FindByLevel(MetadataElement::SYSTEM, "");
    ASSERT_EQUAL(size_t(1), elements->size());
    element = elements->at(0);
    ASSERT_EQUAL(MetadataElement::SYSTEM, element->GetType());
    ASSERT_EQUAL(1, mLoader->GetTotalLoadCount());
    
    elements = mFinder->FindByLevel(MetadataElement::TABLE, "Property:RES");
    ASSERT_EQUAL(size_t(2), elements->size());
    element = elements->at(0);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("ListPrice", element->GetId());
    element = elements->at(1);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("ListDate", element->GetId());
    ASSERT_EQUAL(2, mLoader->GetTotalLoadCount());
    
    elements = mFinder->FindByLevel(MetadataElement::TABLE, "Property:CON");
    ASSERT_EQUAL(size_t(0), elements->size());
    ASSERT_EQUAL(3, mLoader->GetTotalLoadCount());
    
    elements = mFinder->FindByLevel(MetadataElement::UPDATE, "Property:RES");
    ASSERT_EQUAL(size_t(0), elements->size());
    ASSERT_EQUAL(4, mLoader->GetTotalLoadCount());
    
    ASSERT_EQUAL(1, mLoader->GetLoadCount(MetadataElement::SYSTEM));
    ASSERT_EQUAL(0, mLoader->GetLoadCount(MetadataElement::RESOURCE));
    ASSERT_EQUAL(2, mLoader->GetLoadCount(MetadataElement::TABLE));
    ASSERT_EQUAL(1, mLoader->GetLoadCount(MetadataElement::UPDATE));
    
    // Get a few of them again, to make sure that this time no loads are done
    elements = mFinder->FindByLevel(MetadataElement::SYSTEM, "");
    elements = mFinder->FindByLevel(MetadataElement::TABLE, "Property:RES");
    elements = mFinder->FindByLevel(MetadataElement::UPDATE, "Property:RES");
    ASSERT_EQUAL(4, mLoader->GetTotalLoadCount());
    ASSERT_EQUAL(1, mLoader->GetLoadCount(MetadataElement::SYSTEM));
    ASSERT_EQUAL(0, mLoader->GetLoadCount(MetadataElement::RESOURCE));
    ASSERT_EQUAL(2, mLoader->GetLoadCount(MetadataElement::TABLE));
    ASSERT_EQUAL(1, mLoader->GetLoadCount(MetadataElement::UPDATE));
    
}

void CLASS::testFindByPath()
{
    ASSERT_EQUAL(0, mLoader->GetTotalLoadCount());

    MetadataElementPtr element;
    element = mFinder->FindByPath(MetadataElement::TABLE,
                                     "Property:RES", "ListPrice");
    CPPUNIT_ASSERT(element);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("ListPrice", element->GetId());
    ASSERT_EQUAL(1, mLoader->GetTotalLoadCount());
    
    element = mFinder->FindByPath(MetadataElement::TABLE,
                                     "Property:RES", "ListDate");
    CPPUNIT_ASSERT(element);
    ASSERT_EQUAL(MetadataElement::TABLE, element->GetType());
    ASSERT_STRING_EQUAL("ListDate", element->GetId());
    // This one should have been loaded by the previous find
    ASSERT_EQUAL(1, mLoader->GetTotalLoadCount());
    
    element = mFinder->FindByPath(MetadataElement::TABLE,
                                     "Property:CON", "Beds");
    CPPUNIT_ASSERT(!element);
    ASSERT_EQUAL(2, mLoader->GetTotalLoadCount());
    
    ASSERT_EQUAL(0, mLoader->GetLoadCount(MetadataElement::SYSTEM));
    ASSERT_EQUAL(0, mLoader->GetLoadCount(MetadataElement::RESOURCE));
    ASSERT_EQUAL(2, mLoader->GetLoadCount(MetadataElement::TABLE));
    ASSERT_EQUAL(0, mLoader->GetLoadCount(MetadataElement::UPDATE));
}

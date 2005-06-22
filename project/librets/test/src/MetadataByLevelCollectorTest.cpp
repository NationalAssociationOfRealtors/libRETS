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
    CPPUNIT_TEST(testFind);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testFind();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testFind()
{
    MetadataByLevelCollector collector;
    MetadataElementPtr element(new MetadataSystem());
    element->SetLevel("");
    collector.AddElement(element);

    element.reset(new MetadataTable());
    element->SetLevel("Property:RES");
    collector.AddElement(element);

    element.reset(new MetadataTable());
    element->SetLevel("Property:RES");
    collector.AddElement(element);

    MetadataElementListPtr elements =
        collector.Find(MetadataElement::SYSTEM, "");
    ASSERT_EQUAL(size_t(1), elements->size());

    elements = collector.Find(MetadataElement::TABLE, "Property:RES");
    ASSERT_EQUAL(size_t(2), elements->size());

    elements = collector.Find(MetadataElement::TABLE, "Property:CON");
    ASSERT_EQUAL(size_t(0), elements->size());

    elements = collector.Find(MetadataElement::UPDATE, "Property:RES");
    ASSERT_EQUAL(size_t(0), elements->size());
}

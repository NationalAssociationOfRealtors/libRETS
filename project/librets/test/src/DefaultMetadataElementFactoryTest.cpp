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
#include <sstream>
#include <vector>
#include "testUtil.h"
#include "librets/DefaultMetadataElementFactory.h"
#include "librets/MetadataElement.h"
#include "librets/MetadataSystem.h"
#include "librets/MetadataResource.h"
#include "librets/MetadataClass.h"
#include "librets/MetadataTable.h"
#include "librets/MetadataUpdate.h"
#include "librets/MetadataUpdateType.h"
#include "librets/MetadataUpdateHelp.h"
#include "librets/MetadataObject.h"
#include "librets/MetadataSearchHelp.h"
#include "librets/MetadataEditMask.h"
#include "librets/MetadataLookup.h"
#include "librets/MetadataLookupType.h"
#include "librets/MetadataValidationLookup.h"
#include "librets/MetadataValidationLookupType.h"
#include "librets/MetadataValidationExternal.h"
#include "librets/MetadataValidationExternalType.h"
#include "librets/MetadataValidationExpression.h"
#include "librets/MetadataForeignKey.h"
#include "librets/RetsXmlStartElementEvent.h"

using namespace librets;
using namespace std;

#define CLASS_ DefaultMetadataElementFactoryTest

class CLASS_ : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS_);
    CPPUNIT_TEST(testCreateSystem);
    CPPUNIT_TEST(testCreateResource);
    CPPUNIT_TEST(testCreateClass);
    CPPUNIT_TEST(testCreateTable);
    CPPUNIT_TEST(testCreateUpdate);
    CPPUNIT_TEST(testCreateUpdateType);
    CPPUNIT_TEST(testCreateUpdateHelp);
    CPPUNIT_TEST(testCreateObject);
    CPPUNIT_TEST(testCreateSearchHelp);
    CPPUNIT_TEST(testCreateEditMask);
    CPPUNIT_TEST(testCreateLookup);
    CPPUNIT_TEST(testCreateLookupType);
    CPPUNIT_TEST(testCreateValidationLookup);
    CPPUNIT_TEST(testCreateValidationLookupType);
    CPPUNIT_TEST(testCreateValidationExternal);
    CPPUNIT_TEST(testCreateValidationExternalType);
    CPPUNIT_TEST(testCreateValidationExpression);
    CPPUNIT_TEST(testCreateForeignKey);
    CPPUNIT_TEST_SUITE_END();

  public:
    void setUp();

  protected:
    void testCreateSystem();
    void testCreateResource();
    void testCreateClass();
    void testCreateTable();
    void testCreateUpdate();
    void testCreateUpdateType();
    void testCreateUpdateHelp();
    void testCreateObject();
    void testCreateSearchHelp();
    void testCreateEditMask();
    void testCreateLookup();
    void testCreateLookupType();
    void testCreateValidationLookup();
    void testCreateValidationLookupType();
    void testCreateValidationExternal();
    void testCreateValidationExternalType();
    void testCreateValidationExpression();
    void testCreateForeignKey();

    void runTest(RetsXmlStartElementEventPtr startElement,
                 const type_info & typeInfo,
                 MetadataElement::MetadataType type, string level);

    DefaultMetadataElementFactoryPtr mFactory;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS_);

void CLASS_::setUp()
{
    mFactory.reset(new DefaultMetadataElementFactory());
}

void CLASS_::runTest(RetsXmlStartElementEventPtr startElement,
                     const type_info & typeInfo,
                     MetadataElement::MetadataType type, string level)
{
    MetadataElementPtr element = mFactory->CreateMetadataElement(startElement);
    CPPUNIT_ASSERT(element);
    ASSERT_EQUAL(typeInfo.name(), typeid(*element).name());
    ASSERT_STRING_EQUAL(level, element->GetLevel());
}

void CLASS_::testCreateSystem()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-SYSTEM");
    MetadataElementPtr element = mFactory->CreateMetadataElement(startElement);

    runTest(startElement, typeid(MetadataSystem), MetadataElement::SYSTEM,
            "");
}

void CLASS_::testCreateResource()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-RESOURCE");

    runTest(startElement, typeid(MetadataResource), MetadataElement::RESOURCE,
            "");
}

void CLASS_::testCreateClass()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-CLASS");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataClass), MetadataElement::CLASS,
            "Property");
}

void CLASS_::testCreateTable()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-TABLE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Class", "RES");

    runTest(startElement, typeid(MetadataTable), MetadataElement::TABLE,
            "Property:RES");
}

void CLASS_::testCreateUpdate()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-UPDATE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Class", "RES");

    runTest(startElement, typeid(MetadataUpdate), MetadataElement::UPDATE,
            "Property:RES");
}

void CLASS_::testCreateUpdateType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-UPDATE_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Class", "RES");
    startElement->AddAttribute("Update", "Add");

    runTest(startElement, typeid(MetadataUpdateType),
            MetadataElement::UPDATE_TYPE, "Property:RES:Add");
}

void CLASS_::testCreateUpdateHelp()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-UPDATE_HELP");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataUpdateHelp),
            MetadataElement::UPDATE_HELP, "Property");
}

void CLASS_::testCreateObject()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-OBJECT");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataObject),
            MetadataElement::OBJECT, "Property");
}

void CLASS_::testCreateSearchHelp()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-SEARCH_HELP");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataSearchHelp),
            MetadataElement::SEARCH_HELP, "Property");
}

void CLASS_::testCreateEditMask()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-EDITMASK");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataEditMask),
            MetadataElement::EDIT_MASK, "Property");
}

void CLASS_::testCreateLookup()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-LOOKUP");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataLookup),
            MetadataElement::LOOKUP, "Property");
}

void CLASS_::testCreateLookupType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-LOOKUP_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Lookup", "AR");

    runTest(startElement, typeid(MetadataLookupType),
            MetadataElement::LOOKUP_TYPE, "Property:AR");
}

void CLASS_::testCreateValidationLookup()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_LOOKUP");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataValidationLookup),
            MetadataElement::VALIDATION_LOOKUP, "Property");
}

void CLASS_::testCreateValidationLookupType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_LOOKUP_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("ValidationLookup", "School");

    runTest(startElement, typeid(MetadataValidationLookupType),
            MetadataElement::VALIDATION_LOOKUP_TYPE, "Property:School");
}

void CLASS_::testCreateValidationExternal()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_EXTERNAL");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataValidationExternal),
            MetadataElement::VALIDATION_EXTERNAL, "Property");
}

void CLASS_::testCreateValidationExternalType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_EXTERNAL_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("ValidationExternal", "VET1");

    runTest(startElement, typeid(MetadataValidationExternalType),
            MetadataElement::VALIDATION_EXTERNAL_TYPE, "Property:VET1");
}

void CLASS_::testCreateValidationExpression()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_EXPRESSION");
    startElement->AddAttribute("Resource", "Property");

    runTest(startElement, typeid(MetadataValidationExpression),
            MetadataElement::VALIDATION_EXPRESSION, "Property");
}

void CLASS_::testCreateForeignKey()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-FOREIGNKEYS");
    
    runTest(startElement, typeid(MetadataForeignKey),
            MetadataElement::FOREIGN_KEY, "");
}

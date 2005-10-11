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
#include "librets/RetsUnknownMetadataException.h"
#include "librets/ExceptionErrorHandler.h"

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
    CPPUNIT_TEST(testUnknownMetadata);
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
    void testUnknownMetadata();

    MetadataElementPtr runTest(RetsXmlStartElementEventPtr startElement,
                               const type_info & typeInfo,
                               MetadataElement::MetadataType type,
                               string level);

    DefaultMetadataElementFactoryPtr mFactory;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS_);

void CLASS_::setUp()
{
    mFactory.reset(new DefaultMetadataElementFactory());
    mFactory->SetErrorHandler(ExceptionErrorHandler::GetInstance());
}

MetadataElementPtr CLASS_::runTest(RetsXmlStartElementEventPtr startElement,
                                   const type_info & typeInfo,
                                   MetadataElement::MetadataType type,
                                   string level)
{
    MetadataElementPtr element = mFactory->CreateMetadataElement(startElement);
    CPPUNIT_ASSERT(element);
    ASSERT_EQUAL(typeInfo.name(), typeid(*element).name());
    ASSERT_STRING_EQUAL(level, element->GetLevel());
    return element;
}

void CLASS_::testCreateSystem()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-SYSTEM");
    MetadataElementPtr element = 
        runTest(startElement, typeid(MetadataSystem), MetadataElement::SYSTEM,
                "");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testCreateResource()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-RESOURCE");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataResource),
                MetadataElement::RESOURCE, "");
    element->SetAttribute("ResourceID", "Property");
    ASSERT_STRING_EQUAL("Property", element->GetPath());
}

void CLASS_::testCreateClass()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-CLASS");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataClass), MetadataElement::CLASS,
                "Property");
    element->SetAttribute("ClassName", "ResidentialProperty");
    ASSERT_STRING_EQUAL("Property:ResidentialProperty", element->GetPath());
}

void CLASS_::testCreateTable()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-TABLE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Class", "RES");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataTable), MetadataElement::TABLE,
                "Property:RES");
    element->SetAttribute("SystemName", "ListPrice");
    ASSERT_STRING_EQUAL("Property:RES:ListPrice", element->GetPath());
}

void CLASS_::testCreateUpdate()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-UPDATE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Class", "RES");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataUpdate), MetadataElement::UPDATE,
                "Property:RES");
    element->SetAttribute("UpdateName", "Add");
    ASSERT_STRING_EQUAL("Property:RES:Add", element->GetPath());
}

void CLASS_::testCreateUpdateType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-UPDATE_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Class", "RES");
    startElement->AddAttribute("Update", "Add");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataUpdateType),
                MetadataElement::UPDATE_TYPE, "Property:RES:Add");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testCreateUpdateHelp()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-UPDATE_HELP");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataUpdateHelp),
                MetadataElement::UPDATE_HELP, "Property");
    element->SetAttribute("UpdateHelpID", "1");
    ASSERT_STRING_EQUAL("Property:1", element->GetPath());
}

void CLASS_::testCreateObject()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-OBJECT");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataObject),
                MetadataElement::OBJECT, "Property");
    element->SetAttribute("ObjectType", "Photo");
    ASSERT_STRING_EQUAL("Property:Photo", element->GetPath());
}

void CLASS_::testCreateSearchHelp()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-SEARCH_HELP");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataSearchHelp),
                MetadataElement::SEARCH_HELP, "Property");
    element->SetAttribute("SearchHelpID", "1");
    ASSERT_STRING_EQUAL("Property:1", element->GetPath());
}

void CLASS_::testCreateEditMask()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-EDITMASK");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataEditMask),
                MetadataElement::EDIT_MASK, "Property");
    element->SetAttribute("EditMaskID", "1");
    ASSERT_STRING_EQUAL("Property:1", element->GetPath());
}

void CLASS_::testCreateLookup()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-LOOKUP");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataLookup),
                MetadataElement::LOOKUP, "Property");
    element->SetAttribute("LookupName", "AR");
    ASSERT_STRING_EQUAL("Property:AR", element->GetPath());
}

void CLASS_::testCreateLookupType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-LOOKUP_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("Lookup", "AR");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataLookupType),
                MetadataElement::LOOKUP_TYPE, "Property:AR");
    element->SetAttribute("Value", "LOOP");
    ASSERT_STRING_EQUAL("Property:AR:LOOP", element->GetPath());
}

void CLASS_::testCreateValidationLookup()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_LOOKUP");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataValidationLookup),
                MetadataElement::VALIDATION_LOOKUP, "Property");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testCreateValidationLookupType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_LOOKUP_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("ValidationLookup", "School");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataValidationLookupType),
                MetadataElement::VALIDATION_LOOKUP_TYPE, "Property:School");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testCreateValidationExternal()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_EXTERNAL");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataValidationExternal),
                MetadataElement::VALIDATION_EXTERNAL, "Property");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testCreateValidationExternalType()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_EXTERNAL_TYPE");
    startElement->AddAttribute("Resource", "Property");
    startElement->AddAttribute("ValidationExternal", "VET1");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataValidationExternalType),
                MetadataElement::VALIDATION_EXTERNAL_TYPE, "Property:VET1");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testCreateValidationExpression()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-VALIDATION_EXPRESSION");
    startElement->AddAttribute("Resource", "Property");

    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataValidationExpression),
                MetadataElement::VALIDATION_EXPRESSION, "Property");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testCreateForeignKey()
{
    RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
    startElement->SetName("METADATA-FOREIGNKEYS");
    
    MetadataElementPtr element =
        runTest(startElement, typeid(MetadataForeignKey),
                MetadataElement::FOREIGN_KEY, "");
    ASSERT_STRING_EQUAL("", element->GetPath());
}

void CLASS_::testUnknownMetadata()
{
    try
    {
        RetsXmlStartElementEventPtr startElement(new RetsXmlStartElementEvent());
        startElement->SetName("METADATA-UNKNOWN");
        
        MetadataElementPtr element =
            mFactory->CreateMetadataElement(startElement);
        CPPUNIT_FAIL("Should have thrown an exception");
    }
    catch (RetsUnknownMetadataException & e)
    {
        // Expected
        ASSERT_STRING_EQUAL("METADATA-UNKNOWN", e.GetMetadataName());
    }
}

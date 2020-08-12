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
#include "TestMetadataElement.h"
#include "librets/XmlMetadataParser.h"
#include "librets/XmlMetadataElementFactory.h"
#include "librets/MetadataElementCollector.h"
#include "librets/MetadataElement.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsUnknownMetadataException.h"
#include "librets/ExceptionErrorHandler.h"
#include "librets/RetsErrorHandler.h"

using namespace librets;
using namespace std;
namespace b = boost;

#define CLASS XmlMetadataParserTest


class TestElementFactory;
typedef boost::shared_ptr<TestElementFactory> TestElementFactoryPtr;

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testParseSystem);
    CPPUNIT_TEST(testParseSystemWithEmptyComments);
    CPPUNIT_TEST(testParseTabularData);
    CPPUNIT_TEST(testParseRetsResponse);
    CPPUNIT_TEST(testBlankDataTag);
    CPPUNIT_TEST(testUnknownMetadataException);
    CPPUNIT_TEST(testUnknownMetadataIgnored);
    CPPUNIT_TEST(testExtendedCharacter);
    CPPUNIT_TEST_SUITE_END();

 public:
    void setUp();

  protected:
    void testParseSystem();
    void testParseSystemWithEmptyComments();
    void testParseTabularData();
    void testParseRetsResponse();
    void testBlankDataTag();
    void testUnknownMetadataException();
    void testUnknownMetadataIgnored();
    void testExtendedCharacter();
    
    XmlMetadataParserPtr mParser;
    TestElementFactoryPtr mElementFactory;
};

class TestElementFactory :
    public XmlMetadataElementFactory, public MetadataElementCollector
{
  public:
    void SetErrorHandler(RetsErrorHandler * errorHandler);

    MetadataElementPtr CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent);

    void AddElement(MetadataElementPtr element);

    TestMetadataElementList GetCreatedElements() const;

  private:
    RetsErrorHandler * mErrorHandler;
    TestMetadataElementList mElements;
};

typedef boost::shared_ptr<TestElementFactory> TestElementFactoryPtr;

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void TestElementFactory::SetErrorHandler(RetsErrorHandler * errorHandler)
{
    mErrorHandler = errorHandler;
}

MetadataElementPtr TestElementFactory::CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent)
{
    string metadataName = startElementEvent->GetName();
    TestMetadataElementPtr element;
    if (metadataName == "METADATA-UNKNOWN")
    {
        mErrorHandler->HandleUnknownMetadata(metadataName);
    }
    else
    {
        element.reset(new TestMetadataElement());
        element->SetTypeName(startElementEvent->GetName());
    }
    return element;
}

void TestElementFactory::AddElement(MetadataElementPtr element)
{
    TestMetadataElementPtr testElement =
        b::dynamic_pointer_cast<TestMetadataElement>(element);
    mElements.push_back(testElement);
}

TestMetadataElementList TestElementFactory::GetCreatedElements() const
{
    return mElements;
}

void CLASS::setUp()
{
    mElementFactory.reset(new TestElementFactory());
    mParser.reset(new XmlMetadataParser(mElementFactory,
                                        ExceptionErrorHandler::GetInstance()));
    mParser->SetElementFactory(mElementFactory);
}

void CLASS::testParseSystem()
{
    istreamPtr inputStream = getResource("metadata-system.xml");
    mParser->Parse(inputStream);

    TestMetadataElementList elements = mElementFactory->GetCreatedElements();
    ASSERT_EQUAL(size_t(1), elements.size());
    TestMetadataElementPtr element = elements[0];
    ASSERT_STRING_EQUAL("METADATA-SYSTEM", element->GetTypeName());
    ASSERT_STRING_EQUAL("CRT_RETS", element->GetStringAttribute("SystemID"));
    ASSERT_STRING_EQUAL("Center for REALTOR Technology",
                        element->GetStringAttribute("SystemDescription"));
    ASSERT_STRING_EQUAL("The reference implementation of a RETS Server",
                        element->GetStringAttribute("COMMENTS"));
}

void CLASS::testParseSystemWithEmptyComments()
{
    istreamPtr inputStream = getResource("metadata-system-empty-comments.xml");
    mParser->Parse(inputStream);
    
    TestMetadataElementList elements = mElementFactory->GetCreatedElements();
    ASSERT_EQUAL(size_t(1), elements.size());
    TestMetadataElementPtr element = elements[0];
    ASSERT_STRING_EQUAL("METADATA-SYSTEM", element->GetTypeName());
    ASSERT_STRING_EQUAL("RMLS-RETS", element->GetStringAttribute("SystemID"));
    ASSERT_STRING_EQUAL("RMLS RETS System",
                        element->GetStringAttribute("SystemDescription"));
    ASSERT_STRING_EQUAL("", element->GetStringAttribute("COMMENTS"));
}

void CLASS::testParseTabularData()
{
    istreamPtr inputStream = getResource("metadata-class.xml");
    mParser->Parse(inputStream);

    TestMetadataElementList elements = mElementFactory->GetCreatedElements();
    ASSERT_EQUAL(size_t(2), elements.size());
    TestMetadataElementPtr element = elements[0];
    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
    ASSERT_STRING_EQUAL("CON", element->GetStringAttribute("ClassName"));
    ASSERT_STRING_EQUAL("CommonInterest",
                        element->GetStringAttribute("StandardName"));
    ASSERT_STRING_EQUAL("Condos",
                        element->GetStringAttribute("VisibleName"));
    ASSERT_STRING_EQUAL("Condos",
                        element->GetStringAttribute("Description"));
    ASSERT_STRING_EQUAL("100.00.001",
                        element->GetStringAttribute("TableVersion"));
    ASSERT_STRING_EQUAL("Mon, 18 Aug 2003 17:00:00 GMT",
                        element->GetStringAttribute("TableDate"));
    ASSERT_STRING_EQUAL("100.00.001",
                        element->GetStringAttribute("UpdateVersion"));
    ASSERT_STRING_EQUAL("Mon, 18 Aug 2003 17:00:00 GMT",
                        element->GetStringAttribute("UpdateDate"));

    element = elements[1];
    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
    ASSERT_STRING_EQUAL("RES", element->GetStringAttribute("ClassName"));
    ASSERT_STRING_EQUAL("ResidentialProperty",
                        element->GetStringAttribute("StandardName"));
    ASSERT_STRING_EQUAL("Single Family",
                        element->GetStringAttribute("VisibleName"));
    ASSERT_STRING_EQUAL("Single Family",
                        element->GetStringAttribute("Description"));
    ASSERT_STRING_EQUAL("100.00.001",
                        element->GetStringAttribute("TableVersion"));
    ASSERT_STRING_EQUAL("Mon, 18 Aug 2003 17:00:00 GMT",
                        element->GetStringAttribute("TableDate"));
    ASSERT_STRING_EQUAL("100.00.001",
                        element->GetStringAttribute("UpdateVersion"));
    ASSERT_STRING_EQUAL("Mon, 18 Aug 2003 17:00:00 GMT",
                        element->GetStringAttribute("UpdateDate"));
}

void CLASS::testParseRetsResponse()
{
    istreamPtr inputStream = getResource("metadata-response.xml");
    mParser->Parse(inputStream);

    TestMetadataElementList elements = mElementFactory->GetCreatedElements();
    ASSERT_EQUAL(size_t(3), elements.size());
    TestMetadataElementPtr element = elements[0];
    ASSERT_STRING_EQUAL("METADATA-SYSTEM", element->GetTypeName());
    element = elements[1];
    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
    element = elements[2];
    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
}

void CLASS::testBlankDataTag()
{
    istreamPtr inputStream = getResource("metadata-blank-data.xml");
    mParser->Parse(inputStream);
    
    TestMetadataElementList elements = mElementFactory->GetCreatedElements();
    // Only 1 row should appear, as the blank one should be ignored
    ASSERT_EQUAL(size_t(1), elements.size());
    TestMetadataElementPtr element = elements[0];
    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
    ASSERT_STRING_EQUAL("CON", element->GetStringAttribute("ClassName"));
}

void CLASS::testUnknownMetadataException()
{
    try
    {
        istreamPtr inputStream = getResource("metadata-unknown.xml");
        mParser->Parse(inputStream);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch(RetsUnknownMetadataException & e)
    {
        // Expected
        ASSERT_STRING_EQUAL("METADATA-UNKNOWN", e.GetMetadataName());
    }
}

void CLASS::testUnknownMetadataIgnored()
{
    mParser->SetErrorHandler(NullErrorHandler::GetInstance());
    istreamPtr inputStream = getResource("metadata-unknown.xml");
    mParser->Parse(inputStream);
    
    TestMetadataElementList elements = mElementFactory->GetCreatedElements();
    // No rows should be appear, as the error should be ignored
    ASSERT_EQUAL(size_t(0), elements.size());
}

void CLASS::testExtendedCharacter()
{
    /*
     * This represents "nunez" with extended characters for the accented "u",
     * and the "enyay".
     */
    unsigned char nunez [] = {0x4e, 0xc2, 0x9c, 0xc2, 0x96, 0x65, 0x7a,};
    std::string s_nunez(nunez, nunez + sizeof(nunez) / sizeof(nunez[0]));

    mParser->SetEncoding(RETS_XML_DEFAULT_ENCODING);
    
    try
    {
        istreamPtr inputStream = getResource("metadata-extended-char.xml");

        mParser->Parse(inputStream);
        CPPUNIT_FAIL("Should have thrown exception");
    }
    catch (RetsException &)
    {
        // Expected
    }
    
    mParser->SetEncoding(RETS_XML_ISO_ENCODING);
    istreamPtr inputStream = getResource("metadata-extended-char.xml");
    mParser->Parse(inputStream);

    TestMetadataElementList elements = mElementFactory->GetCreatedElements();
    ASSERT_EQUAL(size_t(1), elements.size());
    TestMetadataElementPtr element = elements[0];

    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
    ASSERT_STRING_EQUAL("RES", element->GetStringAttribute("ClassName"));
    ASSERT_STRING_EQUAL("ResidentialProperty",
                        element->GetStringAttribute("StandardName"));
    ASSERT_STRING_EQUAL("Single Family",
                        element->GetStringAttribute("VisibleName"));
    ASSERT_STRING_EQUAL(s_nunez,
                        element->GetStringAttribute("Description"));
    ASSERT_STRING_EQUAL("100.00.001",
                        element->GetStringAttribute("TableVersion"));
    ASSERT_STRING_EQUAL("Mon, 18 Aug 2003 17:00:00 GMT",
                        element->GetStringAttribute("TableDate"));
    ASSERT_STRING_EQUAL("100.00.001",
                        element->GetStringAttribute("UpdateVersion"));
    ASSERT_STRING_EQUAL("Mon, 18 Aug 2003 17:00:00 GMT",
                        element->GetStringAttribute("UpdateDate"));
}


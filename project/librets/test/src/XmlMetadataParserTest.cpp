#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include <vector>
#include "testUtil.h"
#include "librets/XmlMetadataParser.h"
#include "librets/XmlMetadataElementFactory.h"
#include "librets/MetadataElementCollector.h"
#include "librets/MetadataElement.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsException.h"

using namespace librets;
using namespace std;
namespace b = boost;

#define CLASS XmlMetadataParserTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testParseSystem);
    CPPUNIT_TEST(testParseTabularData);
    CPPUNIT_TEST(testParseRetsResponse);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testParseSystem();
    void testParseTabularData();
    void testParseRetsResponse();

};

class TestMetadataElement : public MetadataElement
{
  public:
    virtual MetadataType GetType() const { return SYSTEM; }
    std::string GetTypeName() const;
    void SetTypeName(std::string typeName);

  private:
    std::string mTypeName;

};
typedef boost::shared_ptr<TestMetadataElement> TestMetadataElementPtr;

string TestMetadataElement::GetTypeName() const
{
    return mTypeName;
}

void TestMetadataElement::SetTypeName(string typeName)
{
    mTypeName = typeName;
}

typedef vector<TestMetadataElementPtr> TestMetadataElementList;

class TestElementFactory :
    public XmlMetadataElementFactory, public MetadataElementCollector
{
  public:
    MetadataElementPtr CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent);

    void AddElement(MetadataElementPtr element);

    TestMetadataElementList GetCreatedElements() const;

  private:
    TestMetadataElementList mElements;
};

typedef boost::shared_ptr<TestElementFactory> TestElementFactoryPtr;

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

MetadataElementPtr TestElementFactory::CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent)
{
    TestMetadataElementPtr element(new TestMetadataElement());
    element->SetTypeName(startElementEvent->GetName());
    return element;
}

void TestElementFactory::AddElement(MetadataElementPtr element)
{
    TestMetadataElementPtr testElement =
        b::dynamic_pointer_cast<TestMetadataElement>(element);
    if (!testElement)
    {
        throw RetsException("Unknown element");
    }
    mElements.push_back(testElement);
}

TestMetadataElementList TestElementFactory::GetCreatedElements() const
{
    return mElements;
}

void CLASS::testParseSystem()
{
    TestElementFactoryPtr elementFactory(new TestElementFactory());
    XmlMetadataParser parser(elementFactory);
    parser.SetElementFactory(elementFactory);
    istreamPtr inputStream = getResource("metadata-system.xml");
    parser.Parse(inputStream);

    TestMetadataElementList elements = elementFactory->GetCreatedElements();
    ASSERT_EQUAL(size_t(1), elements.size());
    TestMetadataElementPtr element = elements[0];
    ASSERT_STRING_EQUAL("METADATA-SYSTEM", element->GetTypeName());
    ASSERT_STRING_EQUAL("CRT_RETS", element->GetStringAttribute("SystemID"));
    ASSERT_STRING_EQUAL("Center for REALTOR Technology",
                        element->GetStringAttribute("SystemDescription"));
    ASSERT_STRING_EQUAL("The reference implementation of a RETS Server",
                        element->GetStringAttribute("COMMENTS"));
}

void CLASS::testParseTabularData()
{
    TestElementFactoryPtr elementFactory(new TestElementFactory());
    XmlMetadataParser parser(elementFactory);
    parser.SetElementFactory(elementFactory);
    istreamPtr inputStream = getResource("metadata-class.xml");
    parser.Parse(inputStream);

    TestMetadataElementList elements = elementFactory->GetCreatedElements();
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
    TestElementFactoryPtr elementFactory(new TestElementFactory());
    XmlMetadataParser parser(elementFactory);
    parser.SetElementFactory(elementFactory);
    istreamPtr inputStream = getResource("metadata-response.xml");
    parser.Parse(inputStream);

    TestMetadataElementList elements = elementFactory->GetCreatedElements();
    ASSERT_EQUAL(size_t(3), elements.size());
    TestMetadataElementPtr element = elements[0];
    ASSERT_STRING_EQUAL("METADATA-SYSTEM", element->GetTypeName());
    element = elements[1];
    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
    element = elements[2];
    ASSERT_STRING_EQUAL("METADATA-CLASS", element->GetTypeName());
}

#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "librets/GetObjectRequest.h"
#include "librets/RetsHttpRequest.h"

using namespace librets;
using namespace std;

#define CLASS GetObjectRequestTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testAddOneObject);
    CPPUNIT_TEST(testAddAllObjects);
    CPPUNIT_TEST(testAddMultipleObjects);
    CPPUNIT_TEST(testAddMultipleResourceEntities);
    CPPUNIT_TEST_SUITE_END();
    
  protected:
    void testAddOneObject();
    void testAddAllObjects();
    void testAddMultipleObjects();
    void testAddMultipleResourceEntities();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testAddOneObject()
{
    GetObjectRequest request("Property", "Photo");
    request.AddObject("LN1", 1);
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest();
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:1&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

void CLASS::testAddAllObjects()
{
    GetObjectRequest request("Property", "Photo");
    request.AddAllObjects("LN1");
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest();
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:*&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

void CLASS::testAddMultipleObjects()
{
    GetObjectRequest request("Property", "Photo");
    request.AddObject("LN1", 1);
    request.AddObject("LN1", 2);
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest();
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:1:2&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}

void CLASS::testAddMultipleResourceEntities()
{
    GetObjectRequest request("Property", "Photo");
    request.AddObject("LN1", 1);
    request.AddAllObjects("LN2");
    RetsHttpRequestPtr httpRequest = request.CreateHttpRequest();
    CPPUNIT_ASSERT(httpRequest);
    ASSERT_STRING_EQUAL("ID=LN1:1,LN2:*&Location=0&Resource=Property&Type=Photo",
                        httpRequest->GetQueryString());
}
#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include "testUtil.h"
#include "librets/RetsHttpRequest.h"

using namespace librets;
using namespace std;

#define CLASS RetsHttpRequestTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testQueryStringNoParameters);
    CPPUNIT_TEST(testQueryStringSingleParameter);
    CPPUNIT_TEST(testQueryStringMultipleParameters);
    CPPUNIT_TEST(testQueryStringRemoveParameter);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testQueryStringNoParameters();
    void testQueryStringSingleParameter();
    void testQueryStringMultipleParameters();
    void testQueryStringRemoveParameter();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testQueryStringNoParameters()
{
    RetsHttpRequest request;
    ASSERT_STRING_EQUAL("", request.GetQueryString());
}

void CLASS::testQueryStringSingleParameter()
{
    RetsHttpRequest request;
    request.SetQueryParameter("name", "value");
    ASSERT_STRING_EQUAL("name=value", request.GetQueryString());
}

void CLASS::testQueryStringMultipleParameters()
{
    RetsHttpRequest request;
    request.SetQueryParameter("name", "value");
    request.SetQueryParameter("foo", "bar");
    ASSERT_STRING_EQUAL("foo=bar&name=value", request.GetQueryString());
}

void CLASS::testQueryStringRemoveParameter()
{
    RetsHttpRequest request;
    request.SetQueryParameter("name", "value");
    request.SetQueryParameter("foo", "bar");
    // This should remove name
    request.SetQueryParameter("name", "");
    ASSERT_STRING_EQUAL("foo=bar", request.GetQueryString());
}

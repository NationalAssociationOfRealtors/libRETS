#include <cppunit/extensions/HelperMacros.h>
#include <sstream>
#include "testUtil.h"
#include "librets/LogoutResponse.h"

using namespace librets;
using namespace std;

#define CLASS LogoutResponseTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testValidResponse);
    CPPUNIT_TEST(testShortResponse);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testValidResponse();
    void testShortResponse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testValidResponse()
{
    istreamPtr inputStream = getResource("logout_valid15.xml");
    LogoutResponse response;
    response.Parse(inputStream);

    ASSERT_STRING_EQUAL("51", response.GetValue("ConnectTime"));
    ASSERT_STRING_EQUAL("51", response.GetValue("connecttime"));
    ASSERT_STRING_EQUAL("", response.GetValue("none"));
    
    ASSERT_STRING_EQUAL("0.36", response.GetBillingInfo());
    ASSERT_STRING_EQUAL("Goodbye", response.GetLogoutMessage());
    ASSERT_EQUAL(51, response.GetConnectTime());
}

void CLASS::testShortResponse()
{
    istreamPtr inputStream = getResource("logout-short.xml");
    LogoutResponse response;
    response.Parse(inputStream);
    
}
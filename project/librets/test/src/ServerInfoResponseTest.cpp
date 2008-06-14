/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
#include <stdexcept>
#include "testUtil.h"
#include "librets/ServerInfoResponse.h"
#include "librets/RetsReplyException.h"
#include "librets/EncodingType.h"

using namespace librets;
using namespace std;

#define CLASS ServerInformationResponseTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testValidResponse);
//    CPPUNIT_TEST(testNoClass);
    CPPUNIT_TEST(testCustomResponse);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testCustomResponse();
//    void testNoClass();
    void testValidResponse();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testCustomResponse()
{
    ServerInformationResponse serverResponse;
    istreamPtr inputStream = getResource("server-response-custom.xml");
    serverResponse.Parse(inputStream);
    StringVector parameters = serverResponse.GetParameters();
    ASSERT_EQUAL(StringVector::size_type(1), parameters.size());
    ASSERT_STRING_EQUAL("X-Custom", parameters.at(0));
    ASSERT_STRING_EQUAL("This is a custom field.", serverResponse.GetValue("X-Custom"));
}

void CLASS::testValidResponse()
{
    ServerInformationResponse serverResponse;
    istreamPtr inputStream = getResource("server-response-property.xml");
    serverResponse.Parse(inputStream);
    StringVector parameters = serverResponse.GetParameters();
    ASSERT_EQUAL(StringVector::size_type(4), parameters.size());
    ASSERT_STRING_EQUAL("LastTimeStamp", parameters.at(0));
    ASSERT_STRING_EQUAL("MinimumLimit", parameters.at(1));
    ASSERT_STRING_EQUAL("KeyLimit", parameters.at(2));
    ASSERT_STRING_EQUAL("ReplicationSupport", parameters.at(3));
    
    ASSERT_STRING_EQUAL("2008-06-12T10:30:00", serverResponse.GetValue("LastTimeStamp"));
    ASSERT_STRING_EQUAL("NONE", serverResponse.GetValue("MinimumLimit"));
    ASSERT_STRING_EQUAL("1234", serverResponse.GetValue("KeyLimit"));
    ASSERT_STRING_EQUAL("N", serverResponse.GetValue("ReplicationSupport"));
}




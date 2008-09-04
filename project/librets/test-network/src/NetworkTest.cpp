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
#include "librets.h"

using namespace librets;
using namespace std;

#define CLASS NetworkTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testStreaming);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testStreaming();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

/*
 * This test is intended to run concurrently with the http playback server httpServer.
 * It expects the server to be running on localhost, on port 4444.
 * The two transactions are the login transaction and getmetadata transaction. Login really
 * does not do a login, but is needed to satisfy RetsSession to make it think it has
 * successfully logged in with a server. The getMetadta transaction fetches all the
 * metadata in one transaction. This will exercise the streaming interface since the
 * single POST request will result in multiple packets being served to satisfy the
 * request. The script being played has a known consistent format, so we can verify
 * the first part and the last part. If that works, all the intervening activity,
 * including the multiple selects(), the stringstream subclass CurlStream, and the
 * XML parsing through the multiple packets has worked and state did not get messed
 * up.
 */
void CLASS::testStreaming()
{
    RetsSessionPtr session(new RetsSession("http://localhost:4444/rets/login"));
    //system("cd build/librets/test/bin && java httpServer --port=4444 &");

    session->UseHttpGet(false);
    session->SetIncrementalMetadata(true);

    ASSERT_EQUAL (true, session->Login("Joe", "Blow"));
    std::cout << "Logged in!" << std::endl;

    RetsMetadata * metadata = session->GetMetadata();

    SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","ResidentialProperty",
    								"(ListPrice=300000-)");
    searchRequest->SetSelect("ListingID,ListPrice,Beds,City");
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);

    std::cout << "Doing Search" << std::endl;
    SearchResultSetAPtr results = session->Search(searchRequest.get());

    while (results->HasNext())
    {
        std::cout << "Found entry " << results->GetString("ListingID") << std::endl;
    }
    std::cout <<"Yo! testLogin" << std::endl;
}

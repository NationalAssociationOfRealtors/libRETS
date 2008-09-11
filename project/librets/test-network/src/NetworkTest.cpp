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
    CPPUNIT_TEST(testCurlTimeout);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testStreaming();
    void testCurlTimeout();
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
void CLASS::testCurlTimeout()
{
    RetsSessionPtr session(new RetsSession("http://localhost:4444/rets/login"));
    int total_records = 0;
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);

    ASSERT_EQUAL (true, session->Login("Joe", "Blow"));

    /*
     * Set a very short timeout. If you want to test this in destructive mode
     * (e.g. you want the test to timeout and fail to make sure the timeout
     * actually works), you will need to modify the test to actually talk to 
     * a machine over the network and then do a ping flood as root.
     * For example, given that you are running the test on the machine "alpha"
     * and can run the server on machine "beta", do the following:
     *   Modify the RetsSession above to change localhost to the FQDN for "beta".
     *   On beta, do "make run-server" in one shell.
     *   On beta, as root, do "ping -f alpha" in another shell.
     *   On alpha, run the test.
     */
    session->SetTimeout(1);
    
    /*
     * Perform the search and see if 160 records are returned.
     */
    SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","ResidentialProperty",
    								"(ListPrice=300000-)");
    searchRequest->SetSelect("ListingID,ListPrice,Beds,City");
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);

    SearchResultSetAPtr results = session->Search(searchRequest.get());

    while (results->HasNext())
    {
        total_records++;
    }
    ASSERT_EQUAL (results->GetCount(), total_records);
}

void CLASS::testStreaming()
{
    RetsSessionPtr session(new RetsSession("http://localhost:4444/rets/login"));
    int total_records = 0;
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);

    ASSERT_EQUAL (true, session->Login("Joe", "Blow"));

    /*
     * Fetch and spot check some of the metadata.
     */
    RetsMetadata * metadata = session->GetMetadata();
    MetadataSystem * system = metadata->GetSystem();
    ASSERT_STRING_EQUAL("CRT_RETS", system->GetSystemID());
    
    MetadataResourceList resources = metadata->GetAllResources();
    ASSERT_STRING_EQUAL("Agent", resources[0]->GetId());
    ASSERT_STRING_EQUAL("ActiveAgent", resources[1]->GetId());
    ASSERT_STRING_EQUAL("Office", resources[2]->GetId());
    ASSERT_STRING_EQUAL("Property", resources[3]->GetId());

    /*
     * Perform the search and see if 160 records are returned.
     */
    SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","ResidentialProperty",
    								"(ListPrice=300000-)");
    searchRequest->SetSelect("ListingID,ListPrice,Beds,City");
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);

    SearchResultSetAPtr results = session->Search(searchRequest.get());

    while (results->HasNext())
    {
        total_records++;
    }
    ASSERT_EQUAL (results->GetCount(), total_records);
}


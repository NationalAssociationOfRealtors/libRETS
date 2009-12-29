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
    CPPUNIT_TEST(testCache);
    CPPUNIT_TEST(testNoStreaming);
    CPPUNIT_TEST(test100Continue);
    CPPUNIT_TEST(testRawSearch);
    CPPUNIT_TEST(testRawGetMetadata);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testStreaming();
    void testCurlTimeout();
    void testCache();
    void testNoStreaming();
    void test100Continue();
    void testRawSearch();
    void testRawGetMetadata();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

/*
 * These tests are intended to run concurrently with the http playback server httpServer.
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
    RetsSessionPtr session(new RetsSession(getUrl()));
    int total_records = 0;
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);
    session->SetModeFlags(RetsSession::MODE_NO_SSL_VERIFY);

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
    try
    {
        session->SetTimeout(1);
        /*
         * Perform the search and see if 160 records are returned.
         */
        SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","RES",
                                                                    "(ListPrice=300000-)");
        searchRequest->SetSelect("ListingID,ListPrice,Beds,City");
        searchRequest->SetStandardNames(true);
        searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
        searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
        searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
        searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);
        searchRequest->SetRestrictedIndicator("xxxx");

        SearchResultSetAPtr results = session->Search(searchRequest.get());

        while (results->HasNext())
        {
            total_records++;
        }
        ASSERT_EQUAL (results->GetCount(), total_records);
    }
    catch (RetsHttpException & e)
    {
        ASSERT_EQUAL (408, e.GetHttpResult());
    }
    session->Logout();
}

void CLASS::testStreaming()
{
    RetsSessionPtr session(new RetsSession(getUrl()));
    int total_records = 0;
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);
    session->SetModeFlags(RetsSession::MODE_NO_SSL_VERIFY);

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
    SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","RES",
                                                                    "(ListPrice=300000-)");
    searchRequest->SetSelect("ListingID,ListPrice,Beds,City");
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);
    searchRequest->SetRestrictedIndicator("xxxx");

    SearchResultSetAPtr results = session->Search(searchRequest.get());

    while (results->HasNext())
    {
        total_records++;
    }
    ASSERT_EQUAL (results->GetCount(), total_records);
    session->Logout();
}

void CLASS::testCache()
{
    RetsSessionPtr session(new RetsSession(getUrl()));
    int total_records = 0;
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);
    session->SetModeFlags(RetsSession::MODE_CACHE | RetsSession::MODE_NO_SSL_VERIFY);

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
    SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","RES",
                                                                    "(ListPrice=300000-)");
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);
    searchRequest->SetRestrictedIndicator("xxxx");

    SearchResultSetAPtr results = session->Search(searchRequest.get());

    while (results->HasNext())
    {
        total_records++;
    }
    ASSERT_EQUAL (results->GetCount(), total_records);
    session->Logout();
}

void CLASS::testNoStreaming()
{
    RetsSessionPtr session(new RetsSession(getUrl()));
    int total_records = 0;
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);
    session->SetModeFlags(RetsSession::MODE_NO_STREAM | RetsSession::MODE_NO_SSL_VERIFY);

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
    SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","RES",
                                                                    "(ListPrice=300000-)");
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);
    searchRequest->SetRestrictedIndicator("xxxx");

    SearchResultSetAPtr results = session->Search(searchRequest.get());

    while (results->HasNext())
    {
        total_records++;
    }
    ASSERT_EQUAL (results->GetCount(), total_records);
    session->Logout();
}

void CLASS::test100Continue()
{
    RetsSessionPtr session(new RetsSession("http://demo.crt.realtors.org:6103/rets/login"));
    int total_records = 0;
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);
    session->SetModeFlags(RetsSession::MODE_NO_SSL_VERIFY);

    ASSERT_EQUAL (true, session->Login("Joe", "Schmoe"));

    string query = "(StateOrProvince=\
AL,AK,AS,AZ,AR,CA,CO,CT,\
DE,DC,FM,FL,GA,GU,HI,ID,\
IL,IN,IA,KS,KY,LA,ME,MH,\
MD,MA,MI,MN,MS,MO,MT,NE,\
NV,NH,NJ,NM,NY,NC,ND,MP,\
OH,OK,OR,PW,PA,PR,RI,SC,\
SD,TN,TX,UT,VT,VI,VA,WA,\
WV,WI,WY,\
AL,AK,AS,AZ,AR,CA,CO,CT,\
DE,DC,FM,FL,GA,GU,HI,ID,\
IL,IN,IA,KS,KY,LA,ME,MH,\
MD,MA,MI,MN,MS,MO,MT,NE,\
NV,NH,NJ,NM,NY,NC,ND,MP,\
OH,OK,OR,PW,PA,PR,RI,SC,\
SD,TN,TX,UT,VT,VI,VA,WA,\
WV,WI,WY,\
AL,AK,AS,AZ,AR,CA,CO,CT,\
DE,DC,FM,FL,GA,GU,HI,ID,\
IL,IN,IA,KS,KY,LA,ME,MH,\
MD,MA,MI,MN,MS,MO,MT,NE,\
NV,NH,NJ,NM,NY,NC,ND,MP,\
OH,OK,OR,PW,PA,PR,RI,SC,\
SD,TN,TX,UT,VT,VI,VA,WA,\
WV,WI,WY,\
AL,AK,AS,AZ,AR,CA,CO,CT,\
DE,DC,FM,FL,GA,GU,HI,ID,\
IL,IN,IA,KS,KY,LA,ME,MH,\
MD,MA,MI,MN,MS,MO,MT,NE,\
NV,NH,NJ,NM,NY,NC,ND,MP,\
OH,OK,OR,PW,PA,PR,RI,SC,\
SD,TN,TX,UT,VT,VI,VA,WA,\
WV,WI,WY)";

    string select = "AgentID,Baths,Beds,City,County,ListingID,ListPrice,ListingStatus,StateOrProvince,StreetName,StreetNumber";
    /*
     * This test needs to be periodically checked under tcpdump to confirm it is doing what is is supposed to
     * do. The arguments to the POST request exceed 1,420 characters, which is around the size of an http
     * packet. libCURL switches to the Expect: 100-continue protocol when it will take two packets to send
     * the entire request. We run this test twice ... once with the Expect: 100-continue enabled, one without.
     * In the former case, the tcpdump should indicate that the POST request ends with "Expect: 100-Continue".
     * The server should then send back: "HTTP/1.1 100 Continue" at which point, libCURL will send the balance
     * of the query arguments. In the second case, the query arguments should start in the same packet that
     * contains the POST request and carry into a second packet.
     *
     * This is a live test that runs against http://demo.crt.realtors.org:6103/rets/login.
     */
    SearchRequestAPtr searchRequest = session->CreateSearchRequest("Property","RES",
                                                                    query);
    searchRequest->SetSelect(select);
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);
    searchRequest->SetRestrictedIndicator("xxxx");

    SearchResultSetAPtr results1 = session->Search(searchRequest.get());

    while (results1->HasNext())
    {
        total_records++;
    }
    ASSERT_EQUAL (results1->GetCount(), total_records);
    
    session->SetModeFlags(RetsSession::MODE_NO_EXPECT | RetsSession::MODE_NO_SSL_VERIFY);
    total_records = 0;
    
    SearchResultSetAPtr results2 = session->Search(searchRequest.get());
    while (results2->HasNext())
    {
        total_records++;
    }
    ASSERT_EQUAL (results2->GetCount(), total_records);
    ASSERT_EQUAL (results1->GetCount(), results2->GetCount());
    session->Logout();
}

void CLASS::testRawGetMetadata()
{
    RetsSessionPtr session(new RetsSession(getUrl()));
    
    istreamPtr inputStream = getResource("raw-metadata");
    
    std::stringstream referenceStream;
    referenceStream << inputStream->rdbuf();
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);
    session->SetModeFlags(RetsSession::MODE_NO_SSL_VERIFY);

    ASSERT_EQUAL (true, session->Login("Joe", "Schmoe"));

    std::stringstream testStream;
    session->GetMetadata(testStream);
    
    ASSERT_STRING_EQUAL(referenceStream.str(), testStream.str());

    session->Logout();
}

void CLASS::testRawSearch()
{
    RetsSessionPtr session(new RetsSession(getUrl()));
    istreamPtr inputStream = getResource("raw-search");
    
    std::stringstream referenceStream;
    referenceStream << inputStream->rdbuf();
    
    session->UseHttpGet(false);
    session->SetIncrementalMetadata(false);
    session->SetModeFlags(RetsSession::MODE_NO_SSL_VERIFY);

    ASSERT_EQUAL (true, session->Login("Joe", "Schmoe"));

    SearchRequestAPtr searchRequest = session->CreateSearchRequest(
                                        "Property", "RES",
                                        "(ListPrice=300000-)");
    searchRequest->SetSelect("ListingID,ListPrice,Beds,City");
    searchRequest->SetStandardNames(true);
    searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
    searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
    searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
    searchRequest->SetFormatType(SearchRequest::COMPACT_DECODED);
    searchRequest->SetRestrictedIndicator("xxxx");
    
    std::stringstream testStream;
    session->Search(searchRequest.get(), testStream);

    ASSERT_STRING_EQUAL(referenceStream.str(), testStream.str());

    session->Logout();
}


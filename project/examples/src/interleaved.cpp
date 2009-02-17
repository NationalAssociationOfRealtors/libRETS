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

#include "librets.h"
#include "Options.h"
#include <iostream>
#include <iomanip>
#include <boost/date_time.hpp>
#include "librets/str_stream.h"

using namespace librets;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::stringstream;
using std::setw;
using std::vector;
namespace po = boost::program_options;

int main(int argc, char * argv[])
{
    try
    {
        string                      classTimeStamp;
        SearchRequest::CountType    count;
        string                      countString;
        int                         defaultLimit = SearchRequest::LIMIT_DEFAULT;
        int                         defaultOffset = SearchRequest::OFFSET_NONE;
        SearchRequest::FormatType   format = SearchRequest::COMPACT_DECODED;
        string                      keyField;
        string                      lastModified;
        vector<string>              listingIds;
        int                         limit;
        RetsMetadata *              metadata;
        MetadataClass *             metadataClass;
        MetadataResource *          metadataResource;
        int                         offset;
        bool                        printCount;
        string                      query;
        string                      resource;
        string                      searchClass;
        SearchRequestAPtr           searchRequest;
        string                      select;
        bool                        standardNames = true;
        int                         totalListings = 0;
        string                      type;

        Options options;
        options.descriptions.add_options()
            ("resource,r", po::value<string>(&resource)
             ->default_value("Property"), "Search resource")
            ("class,C", po::value<string>(&searchClass)
             ->default_value("ResidentialProperty"), "Search class")
            ("timestamp,T", po::value<string>(&classTimeStamp)
             ->default_value(""), "Systemname of the Class TimeStamp field")
             ("lastmodified,L", po::value<string>(&lastModified)
              ->default_value(""), "RETS timestamp of the earliest date from which to select")
            ("select,s", po::value<string>(&select)
             ->default_value("ListingID,ListPrice,Beds,City"), "Search select")
            ("query,q", po::value<string>(&query)
             ->default_value("(ListPrice=300000-)"), "Search query")
            ("system-names,S", "Use system names, instead of standard names")
            ("type,t", po::value<string>(&type)
             ->default_value("Photo"), "Media Type")
            ("compact", "Use COMPACT instead of COMPACT-DECODED")
            ("limit,L", po::value<int>(&limit)
             ->default_value(defaultLimit), "Set the limit")
            ("offset,O", po::value<int>(&offset)
             ->default_value(defaultOffset), "Set the offset")
            ("count,n", po::value<string>(&countString)
             ->default_value("yes"),
             "Set the count type: no, yes or count-only)")
            ;
        if (!options.ParseCommandLine(argc, argv))
        {
            return 0;
        }
        if (options.count("system-names"))
        {
            standardNames = false;
        }
        if (options.count("compact"))
        {
            format = SearchRequest::COMPACT;
        }
        
        if (countString == "yes")
        {
            count = SearchRequest::RECORD_COUNT_AND_RESULTS;
            printCount = true;
        }
        else if (countString == "no")
        {
            count = SearchRequest::NO_RECORD_COUNT;
            printCount = false;
        }
        else if (countString == "count-only")
        {
            count = SearchRequest::RECORD_COUNT_ONLY;
            printCount = true;
        }
        else
        {
            count = SearchRequest::RECORD_COUNT_AND_RESULTS;
            printCount = true;
        }

        RetsSessionPtr session = options.RetsLogin();
        if (!session)
        {
            cout << "Login failed\n";
            return -1;
        }
        
        /*
         * Find the keyfield for the resource.
         */
        metadata = session->GetMetadata();
        metadataResource = metadata->GetResource(resource);
        if (metadataResource == NULL)
        {
            cout << "Invalid resource: " << resource << std::endl;
            session->Logout();
            return -1;
        }
        keyField = metadataResource->GetKeyField();
        
        /*
         * Find the timetsamp field if it is known (RETS 1.7 and later). If 
         * not known, the user must provide it.
         */
        metadataClass = metadata->GetClass(resource, searchClass);
        if (metadataClass == NULL)
        {
            cout << "Invalid resource:class: "
                 << resource
                 << ":"
                 << searchClass 
                 << std::endl;
            session->Logout();
            return -1;
        }
        if (classTimeStamp.length() == 0)
            classTimeStamp = metadataClass->GetStringAttribute("ClassTimeStamp");
            
        if (classTimeStamp.length() == 0)
        {
            cout << "Class "
                 << resource
                 << ":"
                 << searchClass
                 << " has no ClassTimeStamp specified in the metadata. "
                 << std::endl
                 << "Please manually provide one using the --timestamp switch."
                 << std::endl;
            session->Logout();
            return -1;
        }
        
        /*
         * See if the last modified timestamp has been provided. If not, take the
         * current time less 24 hours.
         */
        if (lastModified.length() == 0)
        {
            stringstream ss;
            boost::gregorian::date_facet *output_facet = new boost::gregorian::date_facet("%Y-%m-%d");
            ss.imbue(std::locale(std::locale::classic(), output_facet));
            boost::gregorian::date d(boost::gregorian::day_clock::local_day());
            d -= boost::gregorian::days(1);
            ss << d;
            lastModified = ss.str();
        }
        
        /*
         * OK - let's find all listings that have changed since the lastModified date
         */

        /*
         * Construct the query.
         */
        searchRequest = session->CreateSearchRequest(
                                    resource, searchClass,
                                    str_stream()    << "("
                                                    << classTimeStamp
                                                    << "="
                                                    << lastModified
                                                    << "+)");

        //searchRequest->SetSelect(keyField);
        /*
         * Must use system names for this search.
         */
        searchRequest->SetStandardNames(false);
        searchRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
        searchRequest->SetOffset(SearchRequest::OFFSET_NONE);
        searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
        searchRequest->SetFormatType(SearchRequest::COMPACT);
        
        SearchResultSetAPtr results = session->Search(searchRequest.get());
        if (printCount)
        {
            cout << "Matching record count: " << results->GetCount() << endl;
        }
        /*
         * For all listings found, fetch the full listing detail and then the 
         * associated Photos.
         */
        while (results->HasNext())
        {
            totalListings++;
            listingIds.push_back(results->GetString(keyField));
            /*
             * Create a new search to fetch all the detail for the listing.
             */   
            SearchRequestAPtr   listingRequest = session->CreateSearchRequest(
                                                            resource,
                                                            searchClass,
                                                            str_stream()    << "("
                                                                            << keyField
                                                                            << "="
                                                                            << results->GetString(keyField)
                                                                            << ")");
            listingRequest->SetStandardNames(false);
            listingRequest->SetLimit(SearchRequest::LIMIT_DEFAULT);
            listingRequest->SetOffset(SearchRequest::OFFSET_NONE);
            listingRequest->SetCountType(SearchRequest::NO_RECORD_COUNT);
            listingRequest->SetFormatType(SearchRequest::COMPACT);
            
            SearchResultSetAPtr listingResult = session->Search(listingRequest.get());
            StringVector columns = listingResult->GetColumns();
            
            while (listingResult->HasNext())
            {
                /*
                 * Show the listing detail.
                 */
                for (StringVector::iterator i = columns.begin(); i != columns.end(); i++)
                {
                    string column = *i;
                    cout << setw(15) << column << ": "
                        << setw(0) << listingResult->GetString(column) << endl;
                }
            }
            
            /*
             * Now set up to fetch the objects associated with this listing.
             */
            GetObjectRequest getObjectRequest(resource, type);
            getObjectRequest.AddAllObjects(results->GetString(keyField));
            
            GetObjectResponseAPtr   getObjectResponse = session->GetObject(&getObjectRequest);
            
            ObjectDescriptor * objectDescriptor;
            while ((objectDescriptor = getObjectResponse->NextObject()))
            {
                /*
                 * Report the object details.
                 */
                string  objectKey   = objectDescriptor->GetObjectKey();
                int     objectId    = objectDescriptor->GetObjectId();
                string  contentType = objectDescriptor->GetContentType();
                string  description = objectDescriptor->GetDescription();
                int     replyCode   = objectDescriptor->GetRetsReplyCode();
                string  replyText   = objectDescriptor->GetRetsReplyText();
                
                cout << "Object "
                     << objectKey
                     << ":"
                     << objectId;
                if (!description.empty())
                    cout << ", description: "
                         << description;
                cout << endl;
                
                if (replyCode != 0)
                    cout << "***: "
                         << replyCode
                         << ": "
                         << replyText
                         << endl;
            }
            cout << endl;
        }
        cout << "Total Listings Retrieved: " << totalListings << endl;
        for (vector<string>::iterator i = listingIds.begin(); i != listingIds.end(); i++)
            cout << *i << endl;
        
        session->Logout();
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cerr);
    }
    catch (std::exception & e)
    {
        cerr << e.what() << endl;
    }
}

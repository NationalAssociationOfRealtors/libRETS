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

#include "librets.h"
#include "Options.h"
#include <iostream>
#include <iomanip>

using namespace librets;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::setw;
namespace po = boost::program_options;

/*
 * This example demonstrates an advanced use of libRETS where the search results
 * are to be returned to a stream (in this case, a file wrapped by a stream).
 * The stream is later reinjected into the libRETS parse stream. This can allow
 * data to be fetched in one transaction and then later, offline, it can be
 * processed. Note that this only works with COMPACT mode. Any attempt to decode
 * the lookups will fail.
 */

int main(int argc, char * argv[])
{
    try
    {
        string resource;
        string searchClass;
        string select;
        string query;
        bool standardNames = true;
        SearchRequest::FormatType format = SearchRequest::COMPACT_DECODED;
        int limit;
        int offset;
        SearchRequest::CountType count;
        string countString;
        bool printCount;
        string outputFilename;

        // GCC on FC3 for season does not like using
        // SearchRequest::LIMIT_DEFAULT in the default_value calls below.
        // This is a really crappy work around until gcc unhorks itself.
        int defaultLimit = SearchRequest::LIMIT_DEFAULT;
        
        int defaultOffset = SearchRequest::OFFSET_NONE;
        
        Options options;
        options.descriptions.add_options()
            ("resource,r", po::value<string>(&resource)
             ->default_value("Property"), "Search resource")
            ("class,C", po::value<string>(&searchClass)
             ->default_value("ResidentialProperty"), "Search class")
            ("select,s", po::value<string>(&select)
             ->default_value("ListingID,ListPrice,Beds,City"), "Search select")
            ("query,q", po::value<string>(&query)
             ->default_value("(ListPrice=300000-)"), "Search query")
            ("system-names,S", "Use system names, instead of standard names")
            ("compact", "Use COMPACT instead of COMPACT-DECODED")
            ("limit,L", po::value<int>(&limit)
             ->default_value(defaultLimit), "Set the limit")
            ("offset,O", po::value<int>(&offset)
             ->default_value(defaultOffset), "Set the offset")
            ("count,n", po::value<string>(&countString)
             ->default_value("yes"),
             "Set the count type: no, yes or count-only)")
            ("output-file", po::value<string>(&outputFilename)
             ->default_value("searchResults.xml"), "Output file name")
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

        SearchRequestAPtr searchRequest = session->CreateSearchRequest(
            resource, searchClass, query);
        if (select.length() != 0)
            searchRequest->SetSelect(select);
        searchRequest->SetStandardNames(standardNames);
        searchRequest->SetLimit(limit);
        searchRequest->SetOffset(offset);
        searchRequest->SetCountType(count);
        searchRequest->SetFormatType(format);

        /*
         * Perform the search, returning the results to the output file.
         */
        std::ofstream outputStream(outputFilename.c_str());
        session->Search(searchRequest.get(), outputStream);
        outputStream.close();
        
        /*
         * We're done with the session so we can log out.
         */
        session->Logout();

        /*
         * Reopen the file for input.
         */
        istreamPtr inputStream(new std::ifstream(outputFilename.c_str()));

        /*
         * Create the SearchResultSet to handle the parsing of the data.
         */
        SearchResultSetAPtr results(new SearchResultSet());
        results->SetEncoding(options.getEncoding());
        results->SetInputStream(inputStream);
        
        if (printCount)
        {
            cout << "Matching record count: " << results->GetCount() << endl;
        }
        StringVector columns = results->GetColumns();
        while (results->HasNext())
        {
            StringVector::iterator i;
            for (i = columns.begin(); i != columns.end(); i++)
            {
                string column = *i;
                cout << setw(15) << column << ": "
                     << setw(0) << results->GetString(column) << endl;
            }
            cout << endl;
        }
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

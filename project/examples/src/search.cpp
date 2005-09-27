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

int main(int argc, char * argv[])
{
    try
    {
        string resource;
        string searchClass;
        string select;
        string query;
        bool standardNames;
        int limit;

        // GCC on FC3 for season does not like using
        // SearchRequest::LIMIT_DEFAULT in the default_value calls below.
        // This is a really crappy work around until gcc unhorks itself.
        int defaultLimit = SearchRequest::LIMIT_DEFAULT;
        
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
            ("standard-names,n", po::value<bool>(&standardNames)
             ->default_value(true)->implicit(), "Use standard names")
            ("limit,L", po::value<int>(&limit)
             ->default_value(defaultLimit), "Set the limit")
            ;
        if (!options.ParseCommandLine(argc, argv))
        {
            return 0;
        }

        RetsSessionPtr session = options.RetsLogin();
        if (!session)
        {
            cout << "Login failed\n";
            return -1;
        }
        
        SearchRequest searchRequest = session->CreateSearchRequest(
            resource, searchClass, query);
        searchRequest.SetSelect(select);
        searchRequest.SetStandardNames(standardNames);
        searchRequest.SetLimit(limit);
        
        SearchResultSetPtr results = session->Search(searchRequest);
        StringVectorPtr columns = results->GetColumns();
        while (results->HasNext())
        {
            StringVector::iterator i;
            for (i = columns->begin(); i != columns->end(); i++)
            {
                string column = *i;
                cout << setw(15) << column << ": "
                     << setw(0) << results->GetString(column) << endl;
            }
            cout << endl;
        }
        
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

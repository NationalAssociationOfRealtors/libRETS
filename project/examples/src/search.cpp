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

using namespace librets;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char * argv[])
{
    try
    {
        Options options;
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
        
        SearchRequestPtr searchRequest = session->CreateSearchRequest(
            "Property", "ResidentialProperty", "(ListPrice=300000-)");
        
        SearchResultSetPtr results = session->Search(searchRequest);
        while (results->HasNext())
        {
            cout << "ListingID: " << results->GetString("ListingID") << endl;
            cout << "ListPrice: " << results->GetString("ListPrice") << endl;
            cout << "     Beds: " << results->GetString("Beds") << endl;
            cout << "     City: " << results->GetString("City") << endl;
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

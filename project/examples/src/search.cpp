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
#include <iostream>

using namespace librets;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char * argv[])
{
    try
    {
        RetsSessionPtr session(
            new RetsSession("http://demo.crt.realtors.org:6103/rets/login"));
        session->Login("Joe", "Schmoe");
        
        SearchRequestPtr searchRequest(
            new SearchRequest("Property", "ResidentialProperty", 
                              "(ListPrice=300000-)"));
        
        SearchResultSetPtr results = session->Search(searchRequest);
        while (results->HasNext())
        {
            cout << "ListingID: " << results->GetString("ListingID") << endl;
            cout << "ListPrice: " << results->GetString("ListPrice") << endl;
            cout << " Bedrooms: " << results->GetString("Bedrooms") << endl;
            cout << "     City: " << results->GetString("City") << endl;
            cout << endl;
        }
        
        session->Logout();
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cerr);
    }
}

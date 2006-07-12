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
using namespace librets::util;
using namespace std;

void dumpResponse(RetsHttpResponsePtr response);

int main(int argc, char * argv[])
{
    try
    {
        string url = "http://www.example.com/";
        if (argc == 2)
        {
            url = argv[1];
        }

        RetsHttpClientPtr client = RetsHttpClient::CreateDefault();
        RetsHttpRequest request;
        request.SetUrl(url);
        RetsHttpResponsePtr response = client->DoRequest(&request);
        dumpResponse(response);
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cout);
    }
}

void dumpResponse(RetsHttpResponsePtr response)
{
    cout << "Response code: " << response->GetResponseCode() << endl;
    istreamPtr inputStream = response->GetInputStream();
    readUntilEof(*inputStream, cout);
    cout << endl;
}

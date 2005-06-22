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
#include <fstream>
#include <iostream>
#include "librets.h"

using namespace librets;
using namespace librets::util;
using std::istream;
using std::ifstream;
using std::cin;
using std::cout;
using std::endl;

int main(int argc, char * argv[])
{
    try
    {
        SqlToDmqlCompiler compiler;
        istream * inputStream;
        istreamPtr fileInputStream;
        if (argc == 1)
        {
            inputStream = &cin;
        }
        else
        {
            fileInputStream.reset(new ifstream(argv[1]));
            inputStream = fileInputStream.get();
        }

        if (compiler.sqlToDmql(*inputStream) == SqlToDmqlCompiler::DMQL_QUERY)
        {
            DmqlQueryPtr query = compiler.GetDmqlQuery();
            cout << "Resource: " << query->GetResource() << endl;
            cout << "Class: " << query->GetClass() << endl;
            cout << "Fields: " << Output(*query->GetFields()) << endl;
            cout << "DMQL: " << query->GetCriterion()->ToDmqlString() << endl;
        }
        else
        {
            cout << "Not a DMQL query" << endl;
        }
    }
    catch (const RetsException & e)
    {
        e.PrintFullReport(cout);
    }
    
    return 0;
}

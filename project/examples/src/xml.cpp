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
#include <iostream>
#include <fstream>
#include <sstream>
#include "librets.h"

using namespace librets;
using namespace std;
namespace b = boost;

typedef boost::shared_ptr<ifstream> ifstreamPtr;

int main(int argc, char * argv[])
{
    try
    {
        RetsXmlParserPtr xmlParser;

        if (argc == 2)
        {
            ifstreamPtr infile(new ifstream(argv[1]));
            xmlParser = RetsXmlParser::CreateDefault(infile);
        }
        else
        {
            istreamPtr xml(new istringstream(
                "<one attr=\"value\">foo"
                "  <two attr1=\"value1\" attr2=\"value2\">bar</two>"
                "</one>\n"));
            
            xmlParser = RetsXmlParser::CreateDefault(xml);
        }

        while (xmlParser->HasNext())
        {
            RetsXmlEventPtr event = xmlParser->GetNextEvent();
            cout << event << endl;
        }
    }
    catch (RetsException & e)
    {
        cout << "Exception: " << e.GetMessage() << endl;
    }
}

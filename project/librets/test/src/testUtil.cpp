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
#include "testUtil.h"
#include "librets/RetsException.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ifstream;
using std::vector;
using std::endl;
using std::ostringstream;
using std::ios_base;

#define NS librets

typedef boost::shared_ptr<ifstream> ifstreamPtr;

void librets::checkStringEquals(string expected, string actual,
                                CPPUNIT_NS::SourceLine sourceLine)
{
    if (expected == actual)
        return;

    CPPUNIT_NS::Asserter::failNotEqual(expected,
                                       actual,
                                       sourceLine);
}

istreamPtr NS::getResource(string resourceName, ios_base::openmode mode)
{
    string fileName = "project/librets/test/src/" + resourceName;
    ifstreamPtr inputStream(new ifstream(fileName.c_str(), mode));
    if (!(*inputStream))
    {
        throw RetsException("Could not open file: " + fileName);
    }
    return inputStream;
}

void NS::checkVectorEquals(const vector<string> & expected,
                           const vector<string> & actual,
                           CPPUNIT_NS::SourceLine sourceLine)
{
    if (expected != actual)
    {
        ostringstream message;
        message << "Comparison failure: " << endl;
        message << "Expected: " << Output(expected) << endl;
        message << "Actual  : " << Output(actual);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}

void NS::checkVectorEquals(const vector<int> & expected,
                           const vector<int> & actual,
                           CPPUNIT_NS::SourceLine sourceLine)
{
    if (expected != actual)
    {
        ostringstream message;
        message << "Comparison failure: " << endl;
        message << "Expected: " << Output(expected) << endl;
        message << "Actual  : " << Output(actual);
        CPPUNIT_NS::Asserter::fail(
                                   CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}

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
#include <sstream>
#include <iostream>
#include "librets/util.h"
#include "librets/CurlStream.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostringstream;
using std::istringstream;
using std::istream;
using std::ostream;
using std::vector;

#define NS librets::util

const char * NS::WHITESPACE = " \t\n\r";

bool NS::isEmpty(string aString)
{
    string::iterator ch;
    for (ch = aString.begin(); ch != aString.end(); ch++)
    {
        if (!isspace((unsigned char) *ch))
        {
            return false;
        }
    }
    return true;
}

int NS::find(const string & aString, const string & separators,
             int start_index)
{
    int found_index = -1;
    for (unsigned i = 0; i < separators.size(); i++)
    {
        int std_found_index = aString.find(separators[i], start_index);
        if ((std_found_index >= 0) && (found_index == -1))
        {
            found_index = std_found_index;
        }
        else if ((std_found_index >= 0) && (std_found_index < found_index))
        {
            found_index = std_found_index;
        }
    }
    return found_index;
}

string NS::join(const StringVector & strings, string separator)
{
    StringVector::const_iterator i;
    string sep = "";
    string joined;
    for (i = strings.begin(); i != strings.end(); i++)
    {
        joined.append(sep).append(*i);
        sep = separator;
    }
    return joined;
}

string NS::join(string left, string right, string separator)
{
    if (left.empty())
        return right;
    if (right.empty())
        return left;
    return left + separator + right;
}

bool NS::splitField(const string & field, const string & delimiter,
                    string & key, string & value)
{
    string::size_type pos = field.find(delimiter);
    if (pos == string::npos)
    {
        return false;
    }
    key = field.substr(0, pos);
    value = field.substr(pos + 1, string::npos);
    return true;
}

string NS::urlEncode(const string & aString)
{
    string encoded;
    encoded.reserve(aString.size());
    string::const_iterator i;
    for (i = aString.begin(); i != aString.end(); i++)
    {
        switch (*i)
        {
            case '+':
                encoded += "%2b";
                break;
            case '=':
                encoded += "%3d";
                break;
            case '?':
                encoded += "%3f";
                break;
            case '&':
                encoded += "%26";
                break;
            case '%':
                encoded += "%25";
                break;
            default:
               encoded += *i;
        }
    }
    return encoded;
}

void NS::readUntilEof(istreamPtr inputStream, ostream & outputStream)
{
    bool atEof = false;
    bool isCurlStream = (typeid(*inputStream) == typeid(CurlStream));
    int length = 0;

    atEof = isCurlStream ? boost::dynamic_pointer_cast<CurlStream>(inputStream)->eof() 
                         : inputStream->eof();
    while (!atEof)
    {
        char buffer[4096];

        if (isCurlStream)
        {
            boost::dynamic_pointer_cast<CurlStream>(inputStream)->read(buffer, sizeof(buffer));
            length = boost::dynamic_pointer_cast<CurlStream>(inputStream)->gcount();
            atEof = boost::dynamic_pointer_cast<CurlStream>(inputStream)->eof();
        }
        else
        {
            inputStream->read(buffer, sizeof(buffer));
            length = inputStream->gcount();
            atEof = inputStream->eof();
        }

        outputStream.write(buffer, length);
    }
}

void NS::readIntoString(istreamPtr inputStream, string & aString)
{
    bool atEof = false;
    bool isCurlStream = (typeid(*inputStream) == typeid(CurlStream));
    int length = 0;

    atEof = isCurlStream ? boost::dynamic_pointer_cast<CurlStream>(inputStream)->eof() 
                         : inputStream->eof();
    while (!atEof)
    {
        char buffer[4096];

        if (isCurlStream)
        {
            boost::dynamic_pointer_cast<CurlStream>(inputStream)->read(buffer, sizeof(buffer));
            length = boost::dynamic_pointer_cast<CurlStream>(inputStream)->gcount();
            atEof = boost::dynamic_pointer_cast<CurlStream>(inputStream)->eof();
        }
        else
        {
            inputStream->read(buffer, sizeof(buffer));
            length = inputStream->gcount();
            atEof = inputStream->eof();
        }
        aString.append(buffer, length);
    }
}

string NS::readIntoString(istreamPtr inputStream)
{
    string aString;
    readIntoString(inputStream, aString);
    return aString;
}

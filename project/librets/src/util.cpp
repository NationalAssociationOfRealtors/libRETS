#include <sstream>
#include <iostream>
#include "librets/util.h"

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

string NS::trim(const string & aString)
{
    string trimmed = aString;
    // trim leading whitespace
    string::size_type  notwhite = trimmed.find_first_not_of(WHITESPACE);
    trimmed.erase(0, notwhite);

    // trim trailing whitespace
    notwhite = trimmed.find_last_not_of(WHITESPACE);
    trimmed.erase(notwhite + 1);
    return trimmed;
}

bool NS::isEmpty(string aString)
{
    string::iterator ch;
    for (ch = aString.begin(); ch != aString.end(); ch++)
    {
        if (!isspace(*ch))
        {
            return false;
        }
    }
    return true;
}

bool NS::startsWith(string aString, string prefix)
{
    string::size_type pos = aString.find(prefix);
    return (pos == 0);
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

StringVectorPtr NS::split(const string & aString, const string & separators,
                          unsigned limit)
{
    StringVectorPtr strings(new StringVector());
    int start_index = 0;
    int end_index = -1;

    while ((end_index = find(aString, separators, start_index)) != -1)
    {
        if ((limit != 0) && (strings->size() >= (limit - 1)))
        {
            end_index = -1;
            break;
        }
        string field(aString, start_index, (end_index - start_index));
        strings->push_back(field);
        // Put start at character after end
        start_index = end_index + 1;
    }
    string field(aString, start_index, (end_index - start_index));
    strings->push_back(field);
    return strings;
}


StringVectorPtr NS::split_no_empties(const string & aString,
                                     const string & separators)
{
    StringVectorPtr strings(new StringVector());
    int start_index = 0;
    int end_index = -1;

    while ((end_index = find(aString, separators, start_index)) != -1)
    {
        string field(aString, start_index, (end_index - start_index));
        if (!isEmpty(field))
        {
            strings->push_back(field);
        }
        // Put start at character after end
        start_index = end_index + 1;
    }
    string field(aString, start_index, (end_index - start_index));
    if (!isEmpty(field))
    {
        strings->push_back(field);
    }
    return strings;
}

string NS::toUpper(const string & aString)
{
    ToUpper up;
    string s = aString;
    std::transform(s.begin(), s.end(), s.begin(), up);
    return s;
}

string NS::urlEncode(const string & aString)
{
    string encoded;
    encoded.reserve(aString.size());
    string::const_iterator i;
    for (i = aString.begin(); i != aString.end(); i++)
    {
        if (*i == '+')
        {
            encoded += "%2b";
        }
        else
        {
            encoded += *i;
        }
    }
    return encoded;
}

int NS::intValue(const string & aString)
{
    istringstream in(aString);
    int intValue;
    in >> intValue;
    return intValue;
}

string NS::stringValue(int number)
{
    ostringstream out;
    out << number;
    return out.str();
}

void NS::readUntilEof(istream & inputStream, ostream & outputStream)
{
    while (!inputStream.eof())
    {
        char buffer[4096];
        int length;
        inputStream.read(buffer, sizeof(buffer));
        length = inputStream.gcount();
        outputStream.write(buffer, length);
    }
}

string NS::readIntoString(istream & inputStream)
{
    ostringstream buffer;
    readUntilEof(inputStream, buffer);
    return buffer.str();
}

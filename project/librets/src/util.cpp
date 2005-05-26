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

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

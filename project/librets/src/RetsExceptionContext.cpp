#include <boost/filesystem/path.hpp>
#include "librets/RetsExceptionContext.h"

using namespace librets;
using namespace std;
namespace fs = boost::filesystem;

RetsExceptionContext::RetsExceptionContext()
    : mFileName(""), mLineNumber(0)
{
}

RetsExceptionContext::RetsExceptionContext(const string & fileName,
                                           int lineNumber)
{
    fs::path path(fileName, fs::native);
    mFileName = path.leaf();
    mLineNumber = lineNumber;
}

bool RetsExceptionContext::IsValid() const throw()
{
    return (!mFileName.empty());
}

string RetsExceptionContext::GetFileName() const throw()
{
    return mFileName;
}

int RetsExceptionContext::GetLineNumber() const throw()
{
    return mLineNumber;
}


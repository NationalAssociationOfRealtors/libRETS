#include "librets/RetsExceptionContext.h"

using namespace librets;
using namespace std;

RetsExceptionContext::RetsExceptionContext()
{
}

RetsExceptionContext::RetsExceptionContext(const string & fileName,
                                           int lineNumber)
    : mFileName(fileName), mLineNumber(lineNumber)
{
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


#include <iostream>
#include "librets/RetsException.h"

using namespace librets;
using namespace std;

RetsException::RetsException(string message)
{
    mMessage = message;
}

RetsException::~RetsException() throw()
{
}

string RetsException::GetName() const throw()
{
    return "RetsException";
}

void RetsException::SetContext(const RetsExceptionContext & context)
{
    mContext = context;
}

RetsExceptionContext RetsException::GetContext() const throw()
{
    return mContext;
}

string RetsException::GetMessage() const throw()
{
    return mMessage;
}

const char * RetsException::what() const throw()
{
    return mMessage.c_str();
}

void RetsException::PrintFullReport(ostream & out) const throw()
{
    out << GetName();
    if (mContext.IsValid())
    {
        out << " thrown at " << mContext.GetFileName() << ":"
            << mContext.GetLineNumber();
    }
    out << ": " << mMessage << endl;
}

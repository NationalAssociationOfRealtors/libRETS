#include "librets/RetsReplyException.h"

using namespace librets;
using std::string;

RetsReplyException::RetsReplyException(int replyCode, string meaning)
    : RetsException(meaning)
{
    mReplyCode = replyCode;
    mMeaning = meaning;
}

RetsReplyException::~RetsReplyException() throw()
{
}

int RetsReplyException::GetReplyCode() const throw()
{
    return mReplyCode;
}

string RetsReplyException::GetMeaning() const throw()
{
    return mMeaning;
}

#ifndef LIBRETS_RETS_REPLY_EXCEPTION_H
#define LIBRETS_RETS_REPLY_EXCEPTION_H

#include "RetsException.h"

namespace librets {

class RetsReplyException : public RetsException
{
  public:
    RetsReplyException(int replyCode, std::string meaning);

    virtual ~RetsReplyException() throw();

    int GetReplyCode() const throw();

    std::string GetMeaning() const throw();

  private:
    int mReplyCode;
    std::string mMeaning;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

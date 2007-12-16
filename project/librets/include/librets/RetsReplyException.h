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
#ifndef LIBRETS_RETS_REPLY_EXCEPTION_H
#define LIBRETS_RETS_REPLY_EXCEPTION_H

#include "RetsException.h"

namespace librets {

class RetsReplyException : public RetsException
{
  public:
    RetsReplyException(int replyCode, std::string meaning);

    RetsReplyException(int replyCode, std::string meaning, std::string extendedMeaning);

    virtual ~RetsReplyException() throw();

    int GetReplyCode() const throw();

    std::string GetMeaning() const throw();

    std::string GetExtendedMeaning() const throw();
    
  private:
    int mReplyCode;
    std::string mMeaning;
    std::string mExtendedMeaning;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

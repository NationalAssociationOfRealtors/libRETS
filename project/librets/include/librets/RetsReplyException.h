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
/** 
 * @file RetsReplyException.h
 * Contains the RetsReplyException class declaration.
 */

#include "RetsException.h"

namespace librets {

/**
 * RetsReplyException is an implementation of RetsException that is
 * thrown when the RETS REPLY-CODE is non-zero.
 */
class RetsReplyException : public RetsException
{
  public:
    /// @cond MAINTAINER
    /**
     * Construct the exception with the reply code and meaning.
     * @param replyCode The RETS <code>REPLY-CODE</code> sent with the transaction.
     * @param meaning A string containing the meaning of the reply code.
     */
    RetsReplyException(int replyCode, std::string meaning);
    /// @endcond
    
    /// @cond MAINTAINER
    /**
     * Construct the exception with the reply code, meaning and extended meaning.
     * @param replyCode The RETS <code>REPLY-CODE</code> sent with the transaction.
     * @param meaning A string containing the meaning of the reply code.
     * @param extendedMeaning A string containing optional additional text qualifying
     * the exception.
     */
    RetsReplyException(int replyCode, std::string meaning, std::string extendedMeaning);
    /// @endcond
    
    virtual ~RetsReplyException() throw();

    /**
     * Return the RETS <code>REPLY-CODE</code> for this exception.
     * @return An integer representation of the reply code.
     */
    int GetReplyCode() const throw();
    
    /**
     * Return the meaning associate with the reply code.
     * @return A string containing the meaning associated with the reply code.
     */
    std::string GetMeaning() const throw();
    
    /**
     * Return any extended meaning associated with this reply code.
     * @return A string containing the extended meaning. This will be an empty 
     * string if no extended meaning exists.
     */
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

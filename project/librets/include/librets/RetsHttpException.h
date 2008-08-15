/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
#ifndef LIBRETS_RETS_HTTP_EXCEPTION_H
#define LIBRETS_RETS_HTTP_EXCEPTION_H
/** 
 * @file RetsHttpException.h
 * Contains the RetsHttpException class declaration.
 */

#include "RetsException.h"

namespace librets {

/**
 * RetsHttpException is an implementation of RetsException that is
 * thrown when an Http transport error is seen.
 */
class RetsHttpException : public RetsException
{
  public:
    /// @cond MAINTAINER
    /**
     * Construct the exception with the http code and meaning.
     * @param httpResult  The resulting error code from cURL.
     * @param meaning A string containing any additional error information.
     */
    RetsHttpException(int httpResult, std::string meaning);
    /// @endcond
    
    virtual ~RetsHttpException() throw();

    /**
     * Return the Http transaction code.
     * @return An integer representation of the code.
     */
    int GetHttpResult() const throw();
    
    /**
     * Return the additional text.
     * @return A string containing the additional text.
     */
    std::string GetMessage() const throw();
    
    /**
     * Return the name of this exception.
     * @return Always returns "RetsHttpException".
     */
    virtual std::string GetName() const throw();

  private:
    int mHttpResult;
    std::string mMeaning;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

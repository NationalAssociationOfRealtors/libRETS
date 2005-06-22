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
#ifndef LIBRETS_RETS_EXCEPTION_H
#define LIBRETS_RETS_EXCEPTION_H

#include <string>
#include <exception>

#include "librets/RetsExceptionContext.h"

namespace librets {

/**
 * The base class for all libret exceptions.
 */
class RetsException : public std::exception
{
  public:
    /**
     * Creates a new exception with a message string.
     *
     * @param message Exception message
     */
    RetsException(std::string message);

    virtual ~RetsException() throw();
    
    virtual std::string GetName() const throw();

    void SetContext(const RetsExceptionContext & context);
    
    RetsExceptionContext GetContext() const throw();

    /**
     * Returns the error message.
     *
     * @return The error message
     */
    virtual std::string GetMessage() const throw();
    
    virtual void PrintContextMessage(std::ostream & outputStream)
        const throw();

    std::string GetContextMessage() const throw();

    virtual void PrintFullReport(std::ostream & outputStream) const throw();
    
    std::string GetFullReport() const throw();

    /**
     * Returns the error message as a C-style character string.
     *
     * @return The C-style error message
     */
    virtual const char * what() const throw();

  private:
    std::string mMessage;
    RetsExceptionContext mContext;
};

#define LIBRETS_THROW(_ExceptionClass_, _ExceptionConstructor_) \
    { _ExceptionClass_ e _ExceptionConstructor_; \
        e.SetContext(LIBRETS_ECTXT()); \
        throw e; }

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

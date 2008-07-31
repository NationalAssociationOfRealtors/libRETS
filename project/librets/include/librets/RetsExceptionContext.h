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
#ifndef LIBRETS_RETS_EXCEPTION_CONTEXT_H
#define LIBRETS_RETS_EXCEPTION_CONTEXT_H
/**
 * @file RetsExceptionContext.h
 * (Internal) Contains the RetsExceptionContext declaration.
 */
/// @cond MAINTAINER
#include <string>

namespace librets {
/**
 * RetsExceptionContext implements the exception handling context
 * for the library.
 */    
class RetsExceptionContext
{
  public:
    RetsExceptionContext();
    RetsExceptionContext(const std::string & fileName, int lineNumber);
    
    bool IsValid() const throw();
    
    std::string GetFileName() const throw();
    
    int GetLineNumber() const throw();
    
  private:
        std::string mFileName;
    int mLineNumber;
};

#define LIBRETS_ECTXT() RetsExceptionContext(__FILE__, __LINE__)

}
/// @endcond
#endif

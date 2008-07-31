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

#ifndef LIBRETS_RETS_ERROR_HANDLER_H
#define LIBRETS_RETS_ERROR_HANDLER_H
/** 
 * @file RetsErrorHandler.h
 * Contains the RetsErrorHandler class declaration.
 */

#include <string>

namespace librets {
    
/**
 * RetsErrorHandler is an error handling interface that can be used by 
 * users to set error handling policy.
 */
class RetsErrorHandler
{
  public:
    virtual ~RetsErrorHandler();

    /**
     * This gets called during metadata XML parsing if metadata is received
     * using an unknown metadata name.  If the user would like to halt parsing,
     * it should throw a subclass of RetsException.
     *
     * @param name Name of the unknown metadata
     * @throw RetsException if this condition should halt metadata parsing.
     */
    virtual void HandleUnknownMetadata(std::string name) const = 0;
};

/**
 * An implementation of RetsErrorHandler that just silently ignores
 * all errors.
 */
class NullErrorHandler : public RetsErrorHandler
{
  public:
    static NullErrorHandler * GetInstance();
    
    void HandleUnknownMetadata(std::string name) const;
    
  private:
    static NullErrorHandler * sInstance;
};

}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

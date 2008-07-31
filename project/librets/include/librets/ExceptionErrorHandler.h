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

#ifndef LIBRETS_EXCEPTION_ERROR_HANDLER_H
#define LIBRETS_EXCEPTION_ERROR_HANDLER_H
/// @cond MAINTAINER
/** 
 * @file ExceptionErrorHandler.h
 * Contains the RetsMetadata class declaration.
 */

#include "librets/RetsErrorHandler.h"

namespace librets {

/**
 * ExceptionErrorHandler is an implementation of RetsErrorHandler that throws 
 * an exception on error conditions.
 */
class ExceptionErrorHandler : public RetsErrorHandler
{
  public:
    /** 
     * Return the default instance of the error handler.
     * @return A pointer to the default error handler.
     */
    static ExceptionErrorHandler * GetInstance();
    
    /**
     * Throws RetsUnknownMetadataException.
     * @param name A string containing a reference to the unknown metadata.
     * @throw RetsUnknownMetadataException.
     */
    void HandleUnknownMetadata(std::string name) const;
    
  private:
    static ExceptionErrorHandler * sInstance;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

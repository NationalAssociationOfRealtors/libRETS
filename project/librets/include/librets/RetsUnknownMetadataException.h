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

#ifndef LIBRETS_RETS_UNKNOWN_METADATA_EXCEPTION_H
#define LIBRETS_RETS_UNKNOWN_METADATA_EXCEPTION_H
/** 
 * @file RetsUnknownMetadataException.h
 * Contains the RetsUnknownMetadataException class definition.
 */
#include "librets/RetsException.h"

namespace librets {

/**
 * RetsReplyException is an implementation of RetsException that is
 * thrown when the RETS REPLY-CODE is non-zero.
 */
class RetsUnknownMetadataException : public RetsException
{
  public:
    /// @cond MAINTAINER
    /**
     * Construct the exception with the name of the unknown element.
     * @param metadataName A string containing the unknown metadata element.
     */
    RetsUnknownMetadataException(std::string metadataName);
    /// @endcond
    
    virtual ~RetsUnknownMetadataException() throw();

    /**
     * Returns the name of this exception.
     * @return Always returns "RetsUnknownMetadataException"
     */
    virtual std::string GetName() const throw();
    
    /**
     * Returns the name of the unknown element.
     * @return A string containing the name of the unknown element.
     */
    std::string GetMetadataName() const throw();
    
  private:
    std::string mMetadataName;
        
};

};

#endif /* LIBRETS_RETS_UNKNOWN_METADATA_EXCEPTION_H */

/* Local Variables: */
/* mode: c++ */
/* End: */

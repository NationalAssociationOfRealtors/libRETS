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

#ifndef LIBRETS_STREAM_HTTP_LOGGER_H
#define LIBRETS_STREAM_HTTP_LOGGER_H
/** 
 * @file StreamHttpLogger.h
 * (Internal) Contains the Curl Http logging interface class for use with libCURL.
 */
/// @cond MAINTAINER

#include "librets/RetsHttpLogger.h"
#include "librets/std_forward.h"

namespace librets {
/**
 * (Internal) StreamHttpLogger defines the interface to be used with libcurl for logging operations.
 */    
class StreamHttpLogger : public RetsHttpLogger
{
  public:
    StreamHttpLogger(std::ostream * out);

    virtual void SetUseTimestamp(bool useTimestamp);

    virtual void logHttpData(Type type, std::string data);
    
  private:
    Type mLastType;
    std::ostream * mOut;
    bool mUseTimestamp;
};
}
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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

#ifndef LIBRETS_RETS_HTTP_LOGGER_H
#define LIBRETS_RETS_HTTP_LOGGER_H
/** 
 * @file RetsHttpLogger.h
 * (Internal) Contains the Curl Http Logging class for use with libCURL.
 */
/// @cond MAINTAINER

#include <string>

namespace librets {
/**
 * (Internal) RetsHttpLogger is the super class that defines those operations that can be
 * logged by libcurl when the verbose option is enabled.
 */
class RetsHttpLogger
{
  public:
    enum Type
    {
        RECEIVED,
        SENT,
        INFORMATIONAL
    };
    
    virtual ~RetsHttpLogger();

    virtual void logHttpData(Type type, std::string data) = 0;
};
/**
 * NullHttpLogger is a Null RetsHttpLogger class that is used with libcurl when
 * no logging is desired.
 */
class NullHttpLogger : public RetsHttpLogger
{
  public:
    static NullHttpLogger * GetInstance();
    
    void logHttpData(Type type, std::string data);
    
  private:
    static NullHttpLogger * sInstance;
};

}
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

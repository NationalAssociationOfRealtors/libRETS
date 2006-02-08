/*
 * Copyright (C) 2006 National Association of REALTORS(R)
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
 
#ifndef LIBRETS_SHARP_H
#define LIBRETS_SHARP_H

#include "librets/std_forward.h"
#include "librets/RetsHttpLogger.h"

namespace librets {

class InputStreamBridge
{
  public:
    InputStreamBridge(std::istream * inputStream);
    
    int readByte() const;
    
    int read(unsigned char buffer[], int offset, int length) const; 
    
  private:
    std::istream * mInputStream;
};

typedef std::auto_ptr<InputStreamBridge> InputStreamBridgeAPtr;

typedef void (__stdcall* RetsHttpLoggerCallback)(RetsHttpLogger::Type type,
    void * data, int length);

class RetsHttpLoggerBridge : public RetsHttpLogger
{
  public:
    RetsHttpLoggerBridge(RetsHttpLoggerCallback loggerCallback);
    virtual void logHttpData(Type type, std::string data);

  private:
    RetsHttpLoggerCallback mLoggerCallback;
};

};

#endif

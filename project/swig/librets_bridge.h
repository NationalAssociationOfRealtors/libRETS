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
 
#ifndef LIBRETS_BRIDGE_H
#define LIBRETS_BRIDGE_H

/* calling conventions for Windows */
#ifndef SWIGSTDCALL
# if defined(_WIN32) || defined(__WIN32__) || defined(__CYGWIN__)
#   define SWIGSTDCALL __stdcall
# else
#   define SWIGSTDCALL
# endif 
#endif

#include "librets/std_forward.h"

namespace librets {

class InputStreamBridge
{
  public:
    InputStreamBridge(istreamPtr inputStream);
    
    int readByte() const;
    
    std::size_t read(unsigned char buffer[], std::size_t offset, std::size_t length) const; 

  private:
    istreamPtr  mInputStream;
};

typedef std::auto_ptr<InputStreamBridge> InputStreamBridgeAPtr;

class OutputStreamBridge
{
  public:
    OutputStreamBridge(ostreamPtr outputStream);
    
    int flush(void) const;

    int writeByte(char theByte) const;

    std::size_t write(unsigned char buffer[], std::size_t offset, std::streamsize length) const;

  private:
    ostreamPtr  mOutputStream;
};

typedef std::auto_ptr<OutputStreamBridge> OutputStreamBridgeAPtr;

};

#endif

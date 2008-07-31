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

#ifndef LIBRETS_STR_STREAM_H
#define LIBRETS_STR_STREAM_H
/**
 * @file str_stream.h
 * (Internal) Contains the str-stream class definition.
 */
/// @cond MAINTAINER
#include <string>
#include <sstream>

namespace librets
{

class str_stream
{
  public:
    str_stream() : mStreamOut() { }

    // This copy constructor is necessary because stringstream does
    // not provide a copy constructor.  gcc 3.4 transparently copies
    // str_streams, and hence triggers this situation.
    str_stream(const str_stream & s) : mStreamOut(s.mStreamOut.str()) { }

    std::stringstream & underlying_stream() const
    { return mStreamOut; }

    operator std::string() const
    {
        return mStreamOut.str();
    }

  private:
    mutable std::stringstream mStreamOut;
};

template <class type>
    const str_stream & operator<< (const str_stream & out, const type & value)
{
    out.underlying_stream() << value;
    return out;
}
    

}
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

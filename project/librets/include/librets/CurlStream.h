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

#ifndef LIBRETS_CURL_STREAM_H
#define LIBRETS_CURL_STREAM_H

#include <sstream>
#include <iostream>
#include "librets/curl.h"
#include "librets/CurlHttpClient.h"

namespace librets {

class CurlStream : public std::stringstream
{
  public:
    CurlStream(CurlHttpClient& httpClient);

/*    
    CurlStream(std::string & str);
 
    CurlStream(const CurlStream & s);
*/
    
    std::stringstream & underlying_stream() const
    {
        return mCurlStream;
    }
    
    operator std::string() const
    {
        return mCurlStream.str();
    }

    bool eof();

    std::istream& read (char* s , std::streamsize n );

//    std::ostream& write ( const char* s , std::streamsize n );

int get() { printf ("get()\n"); return std::stringstream::get(); }

std::istream& get ( char& c ) { printf ("get(char&c}\n"); return std::stringstream::get(c); }
 
std::istream& get (char* s, std::streamsize n ) { printf ("get(char*s, streamsize n)\n"); return std::stringstream::get(s,n);}

std::istream& get (char* s, std::streamsize n, char delim ) { printf ("istream& get (char* s, streamsize n, char delim );\n"); return std::stringstream::get(s,n,delim); }

std::istream& get (std::streambuf& sb) { printf ("get (streambuf&sb)\n"); return std::stringstream::get(sb); }

std::istream& get (std::streambuf& sb, char delim ) { printf ("get(streambuf& sb, char delim)\n"); return std::stringstream::get (sb, delim); }

std::istream& getline (char* s, std::streamsize n ) { printf ("getline(char* s, streamsize n)\n"); return std::stringstream::getline(s, n); }

std::istream& getline (char* s, std::streamsize n, char delim ) { printf ("getline(char *s, streamsize n, char delim)\n"); return std::stringstream::getline(s,n,delim); }

std::streamsize readsome ( char* s, std::streamsize n ) { printf ("readsome(char *s, streamsize n);\n"); return std::stringstream::readsome(s,n); }

std::stringbuf* rdbuf ( ) const { printf ("rdbuf()\n"); return std::stringstream::rdbuf(); }
    
private:
    mutable std::stringstream mCurlStream;
    CurlHttpClient& mHttpClient;
};

#if 0
template <class type>
    const CurlStream & operator<< (const CurlStream & out, const type & value)
{
    out.underlying_stream() << value;
    return out;
}
#endif

}

#endif

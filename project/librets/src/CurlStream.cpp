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

#include <sstream>
#include "librets/CurlStream.h"

using namespace librets;
using std::string;
using std::ostringstream;

CurlStream::CurlStream(CurlHttpClient& httpClient) 
                        : mHttpClient(httpClient)
                        , mCurlStream()
{
}

/*
CurlStream::CurlStream(std::string & str) 
                        : mHttpClient()
                        , mCurlStream(str)
{
}
*/

/*
CurlStream::CurlStream(const CurlStream & s) : mCurlStream(s.mCurlStream.str())
{
}
*/
std::istream& CurlStream::read(char* s, std::streamsize n)
{
    /*
     * See if the requested data exceeds what is already in the buffer.
     */
    while ((n > (std::stringstream::tellp() - std::stringstream::tellg())) && 
        mHttpClient.ContinueRequest());
    
    return std::stringstream::read(s,n);
//    return *this;
}

bool CurlStream::eof() 
{ 
    bool atEof = std::stringstream::eof();
    
    if (atEof)
    {
        mHttpClient.ContinueRequest();
        
        return std::stringstream::eof(); 
    }
    
    return atEof;
}



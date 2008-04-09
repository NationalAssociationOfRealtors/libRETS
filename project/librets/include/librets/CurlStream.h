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
    /**
     * This class inherits from std::stringstream and overrides the
     * eof() and read() methods in order to work in concenrt with
     * class CurlHttpClient and the cURL multi interface to imlement
     * the streaming interface.
     *
     * @param httpClient A reference to the CurlHttpClient for this call.
     */
    CurlStream(CurlHttpClient& httpClient);

    /**
     * Determine if there is any more data. A side effect of this call is that
     * the CurlHttpClient may be invoked to fetch further data from the network.
     *
     * @return True if there is no more data on the stream.
     */
    bool eof();

    /**
     * Read a block of data from the stream. A side effect of this call is that
     * the CurlHttpClient may be invoked to fetch further data from the network.
     *
     * @param s A pointer to a buffer to which the data will be returned.
     * @param n The maximum size of the buffer.
     * @return The result of the std::stringstream::read() call.
     */
     
    std::istream& read (char* s , std::streamsize n );

private:
    CurlStream();
    
    mutable std::stringstream mCurlStream;
    CurlHttpClient& mHttpClient;
};


}

#endif

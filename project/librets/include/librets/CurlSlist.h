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
#ifndef LIBRETS_CURL_SLIST_H
#define LIBRETS_CURL_SLIST_H
/** 
 * @file CurlSlist.h
 * (Internal) Contains the Curl Http Slist interface class for use with libCURL.
 */

/// @cond MAINTAINER

struct curl_slist;

namespace librets {
/**
 * (Internal) CurlSlist is a class that wraps the libcurl curl_slist type. Refer to the
 * libcurl documentation.
 */
class CurlSlist
{
  public:

    CurlSlist();

    ~CurlSlist();

    void append(const char * string);
    
    void free_all();

    const curl_slist * slist();
    
    void set(curl_slist *slist);

  private:
    curl_slist * mSlist;
};


};
///@endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

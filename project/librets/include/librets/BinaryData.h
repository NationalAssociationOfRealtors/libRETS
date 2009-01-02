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

#ifndef LIBRETS_BINARY_DATA_H
#define LIBRETS_BINARY_DATA_H
/**
 * @file BinaryData.h 
 * Contains class used to wrap binary data returned from the server.
 */

#include <string>
#include "librets/RetsObject.h"
#include "librets/std_forward.h"

namespace librets {
    
/**
 * A helper class that wraps binary data.
 * The BinaryData class wraps binary data returned from the RETS server
 * into a string.
 */
class BinaryData
{
  public:
    /**
     * Default Constructor.
     */
    BinaryData() {};
    
    /**
     * Construct and initialize from an array.
     */
    BinaryData(unsigned char buffer[], int len);
    
    /**
     * Report the size of the media object.
     * @return int representing the size in bytes of the media object.
     */
    int Size() const;
    /**
     * Obtain the media as a string.
     * @return std::string representing the media object.
     */
    std::string AsString() const;
    /**
     * Obtain the media as an array of characters.
     * @return pointer to the media.
     */
    const char * AsChar() const;
    /**
     * Make a copy of the data as the media object.
     * @param buffer[] An array of characters containing the media object.
     * @param length An int representing the length of the media object in bytes.
     */
    void Copy(unsigned char buffer[], int length) const; 
    /**
     * Load the data contained in the input stream as the media object.
     * @param inputStream A pointer to the input stream.
     */
    void ReadToEof(istreamPtr inputStream);
    
  private:
    std::string mData;
};

typedef std::auto_ptr<BinaryData> BinaryDataAPtr;

}

#endif

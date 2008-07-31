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

#ifndef LIBRETS_GET_OBJECT_RESPONSE_H
#define LIBRETS_GET_OBJECT_RESPONSE_H

/** 
 * @file GetObjectResponse.h
 * Contains the GetObjectResponse class definition.
 */

#include <vector>
#include "librets/RetsObject.h"
#include "librets/http_forward.h"
#include "librets/protocol_forward.h"
#include "librets/std_forward.h"

namespace librets {

/**
 * A GetObjectResponse represents a response from a RETS GetObject
 * transaction.  A response may result in zero or more objects.  RETS
 * does not provide a mechanism for getting all objects returned in a
 * response, so a user must retrieve each object in turn, until there
 * are none left.
 */
class GetObjectResponse : public RetsObject
{
  public:
    /**
     * Default constructor.
     */
    GetObjectResponse();
    
    virtual ~GetObjectResponse();
    /// @cond MAINTAINER
    /**
     * (Internal) Set the default object key and object ID.
     * @param defaultObjectKey A string representing the default object key.
     * @param defaultObjectId An integer representing the default object ID.
     */
    void SetDefaultObjectKeyAndId(std::string defaultObjectKey,
                                  int defaultObjectId);
    
    /**
     * (Internal) Parse the response from the server into one or more
     * objects.
     * @param httpResponse A pointer to RetsHttpResponse containing the response
     * from the GetObject request.
     * @param ignoreMalformedHeaders A boolean that when <code>true</code> indicates
     * to the parser that malformed headers and expected and to ignore them.
     */
    void Parse(RetsHttpResponsePtr httpResponse,
               bool ignoreMalformedHeaders = false);
    /// @endcond
    
    /**
     * Returns the next object found in the response.
     *
     * @return The object descriptor representing an object.
     */
    ObjectDescriptor * NextObject();
    
  private:
    typedef std::vector<ObjectDescriptorPtr> ObjectList;
    
    void ParseSinglePart(RetsHttpResponsePtr httpResponse);
    void ParseMultiPart(RetsHttpResponsePtr httpResponse,
                        bool ignoreMalformedHeaders = false);
    
    std::string FindBoundary(std::string contentType);
    void ParsePartStream(istreamPtr in, bool ignoreMalformedHeaders = false);
    
    bool mDefaultsAreValid;
    std::string mDefaultObjectKey;
    int mDefaultObjectId;
    ObjectList mObjects;
    ObjectList::iterator mNextObject;
};
    
}

#endif

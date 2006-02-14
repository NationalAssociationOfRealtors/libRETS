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

#include <vector>
#include "librets/RetsObject.h"
#include "librets/http_forward.h"
#include "librets/protocol_forward.h"
#include "librets/std_forward.h"

namespace librets {

/**
 * The GetObjectResponse represents a response from a RETS GetObject
 * transaction.  A response may result in zero or more objects.  RETS
 * does not provide a mechanism for getting all objects returned in a
 * response, so a user must retrieve each object in turn, until there
 * are none left.
 */
class GetObjectResponse : public RetsObject
{
  public:
    GetObjectResponse();
    
    virtual ~GetObjectResponse();
    
    void SetDefaultObjectKeyAndId(std::string defaultObjectKey,
                                  int defaultObjectId);
    
    void Parse(RetsHttpResponsePtr httpResponse);
    
    /**
     * Returns the next object found in the response.
     *
     * @return The object descriptor representing an object.
     */
    ObjectDescriptor * NextObject();
    
  private:
    typedef std::vector<ObjectDescriptorPtr> ObjectList;
    
    void ParseSinglePart(RetsHttpResponsePtr httpResponse);
    void ParseMultiPart(RetsHttpResponsePtr httpResponse);
    
    std::string FindBoundary(std::string contentType);
    void ParsePartStream(istreamPtr in);
    
    bool mDefaultsAreValid;
    std::string mDefaultObjectKey;
    int mDefaultObjectId;
    ObjectList mObjects;
    ObjectList::iterator mNextObject;
};
    
}

#endif

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
#ifndef LIBRETS_GET_OBJECT_REQUEST_H
#define LIBRETS_GET_OBJECT_REQUEST_H

#include "librets/RetsObject.h"
#include "librets/http_forward.h"
#include "librets/std_forward.h"

namespace librets {
    
class GetObjectRequest : public RetsObject
{
  public:
    GetObjectRequest(std::string resource, std::string type);
    
    virtual ~GetObjectRequest();
    
    bool GetLocation() const;

    void SetLocation(bool location);
    
    void AddObject(std::string resourceEntity, int objectId);
    
    void AddAllObjects(std::string resourceEntity);
    
    RetsHttpRequestPtr CreateHttpRequest() const;
    
    void PrepareHttpRequest(RetsHttpRequestPtr httpRequest) const;
    
  private:
    typedef std::map<std::string, StringVectorPtr> ObjectList;
    
    StringVectorPtr GetObjectIds(std::string resourceEntity);
    
    std::string mResource;
    std::string mType;
    bool mLocation;
    ObjectList mObjectList;
};
    
}

#endif

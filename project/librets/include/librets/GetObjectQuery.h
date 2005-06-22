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
#ifndef LIBRETS_GET_OBJECT_QUERY_H
#define LIBRETS_GET_OBJECT_QUERY_H

#include "librets/std_forward.h"
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"

namespace librets {
    
class GetObjectQuery : public RetsObject
{
  public:
    GetObjectQuery();
    
    std::string GetResource() const;
    
    void SetResource(std::string resource);
    
    std::string GetType() const;
    
    void SetType(std::string type);
    
    std::string GetObjectKey() const;
    
    void SetObjectKey(std::string objectKey);
    
    IntVectorPtr GetObjectIds() const;
    
    void AddObjectId(int objectId);
    
    virtual std::ostream & Print(std::ostream & outputStream) const;

    
  private:
    IntVectorPtr mObjectIds;
    std::string mResource;
    std::string mType;
    std::string mObjectKey;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

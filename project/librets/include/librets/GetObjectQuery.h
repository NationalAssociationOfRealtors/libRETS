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
/** 
 * @file GetObjectQuery.h
 * (Internal) Contains the GetObjectQuery class.
 */
 /// @cond MAINTAINER

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
    
    StringVectorPtr GetObjectKeys() const;
    
    void AddObjectKey(std::string objectKey);
    
    /**
     * Returns a pointer to the StringVector.  GetObjectQuery is
     * responsible for cleaning up the object.
     */
    StringVector * GetObjectKeysPtr() const;
    
    bool GetUseLocation() const;

    void SetUseLocation(bool useLocation);

    virtual std::ostream & Print(std::ostream & outputStream) const;

    /**
     * When you see this value in the ObjectIds, it means that it is a
     * place holder value that should be replaced.
     */
    static const int SUBSTITUTE_VALUE = -99;
    
  private:
    IntVectorPtr mObjectIds;
    StringVectorPtr mObjectKeys;
    std::string mResource;
    std::string mType;
    bool mUseLocation;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

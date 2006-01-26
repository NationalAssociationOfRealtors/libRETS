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
#ifndef LIBRETS_OBJECT_DESCRIPTOR_H
#define LIBRETS_OBJECT_DESCRIPTOR_H

#include <string>
#include "librets/RetsObject.h"
#include "librets/std_forward.h"

namespace librets {
	
class BinaryData
{
  public:
	int Size() const;
	std::string AsString() const;
	const char * AsChar() const;
	void Copy(unsigned char buffer[], int length) const; 
	void ReadToEof(std::istream & inputStream);
	
  private:
	std::string mData;
};

typedef std::auto_ptr<BinaryData> BinaryDataAPtr;

class ObjectDescriptor : public RetsObject
{
  public:
    
    std::string GetObjectKey() const;
    
    void SetObjectKey(std::string objectKey);
    
    int GetObjectId() const;
    
    void SetObjectId(int objectId);
    
    std::string GetDescription() const;
    
    void SetDescription(std::string description);

    std::string GetLocationUrl() const;	
    
    void SetLocationUrl(std::string locationUrl);
    
    std::string GetContentType() const;
    
    void SetContentType(std::string contentType);
    
    istreamPtr GetDataStream();
    
    void SetDataStream(istreamPtr dataStream);
	
	BinaryDataAPtr GetData();
    
  private:
    std::string mObjectKey;
    int mObjectId;
    std::string mDescription;
    std::string mLocationUrl;
    std::string mContentType;
    istreamPtr mDataStream;
};
    
}

#endif

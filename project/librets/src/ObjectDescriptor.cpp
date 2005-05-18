/*
 *  ObjectDescriptor.cpp
 *  librets
 *
 *  Created by Dave Dribin on 4/1/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include "librets/ObjectDescriptor.h"

using namespace librets;
using std::string;

string ObjectDescriptor::GetObjectKey() const
{
    return mObjectKey;
}

void ObjectDescriptor::SetObjectKey(string objectKey)
{
    mObjectKey = objectKey;
}

int ObjectDescriptor::GetObjectId() const
{
    return mObjectId;
}

void ObjectDescriptor::SetObjectId(int objectId)
{
    mObjectId = objectId;
}

string ObjectDescriptor::GetDescription() const
{
    return mDescription;
}

void ObjectDescriptor::SetDescription(string description)
{
    mDescription = description;
}

string ObjectDescriptor::GetLocationUrl() const
{
    return mLocationUrl;
}

void ObjectDescriptor::SetLocationUrl(string locationUrl)
{
    mLocationUrl = locationUrl;
}

string ObjectDescriptor::GetContentType() const
{
    return mContentType;
}

void ObjectDescriptor::SetContentType(string contentType)
{
    mContentType = contentType;
}

istreamPtr ObjectDescriptor::GetData()
{
    return mData;
}

void ObjectDescriptor::SetData(istreamPtr data)
{
    mData = data;
}
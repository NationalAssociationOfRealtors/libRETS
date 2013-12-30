/*
 * Copyright (C) 2005-2013 National Association of REALTORS(R)
 * Portions Copyright (C) 2014 Real Estate Standards Organziation
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
/*
 *  ObjectDescriptor.cpp
 *  librets
 *
 *  Created by Dave Dribin on 4/1/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <iostream>
#include "librets/ObjectDescriptor.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::istream;

ObjectDescriptor::ObjectDescriptor()
{
    mObjectId = 0;
    mDescription = "";
    mContentType = "";
    mPreferred = false;
    mRetsReplyCode = 0;
    mRetsReplyText = "";
    mSubDescription ="";
    mUID = "";
    mWildIndicator = false;
}

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

istreamPtr ObjectDescriptor::GetDataStream()
{
    return mDataStream;
}

void ObjectDescriptor::SetDataStream(istreamPtr dataStream)
{
    mDataStream  = dataStream;
}

BinaryDataAPtr ObjectDescriptor::GetData()
{
    BinaryDataAPtr data(new BinaryData());
    data->ReadToEof(mDataStream);
    return data;
}

ObjectData& ObjectDescriptor::GetObjectData()
{
    return mObjectData;
}

bool ObjectDescriptor::GetPreferred() const
{
    return mPreferred;
}

void ObjectDescriptor::SetPreferred(bool preferred)
{
    mPreferred = preferred;
}

int ObjectDescriptor::GetRetsReplyCode() const
{
    return mRetsReplyCode;
}

void ObjectDescriptor::SetRetsReplyCode(int retsReplyCode)
{
    mRetsReplyCode = retsReplyCode;
}
    
std::string ObjectDescriptor::GetRetsReplyText() const
{
    return mRetsReplyText;
}
  
void ObjectDescriptor::SetRetsReplyText(std::string replyText)
{
    mRetsReplyText = "";
    if (replyText.length())
        mRetsReplyText = replyText;
}

std::string ObjectDescriptor::GetSubDescription() const
{
    return mSubDescription;
}

void ObjectDescriptor::SetSubDescription(string subDescription)
{
    mSubDescription = subDescription;
}

std::string ObjectDescriptor::GetUID() const
{
    return mUID;
}

void ObjectDescriptor::SetUID(string UID)
{
    mUID = UID;
}

bool ObjectDescriptor::GetWildIndicator() const
{
    return mWildIndicator;
}

void ObjectDescriptor::SetWildIndicator(bool wild)
{
    mWildIndicator = wild;
}

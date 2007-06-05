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

#include <iostream>
#include <boost/lexical_cast.hpp>
#include "librets/GetObjectRequest.h"
#include "librets/RetsHttpRequest.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using boost::lexical_cast;

typedef GetObjectRequest CLASS;

const char * CLASS::PHOTO_TYPE = "Photo";
const char * CLASS::PLAT_TYPE = "Plat";
const char * CLASS::VIDEO_TYPE = "Video";
const char * CLASS::AUDIO_TYPE = "Audio";
const char * CLASS::THUMBNAIL_TYPE = "Thumbnail";
const char * CLASS::MAP_TYPE = "Map";
const char * CLASS::VRIMAGE_TYPE = "VRImage";

GetObjectRequest::GetObjectRequest(string resource, string type)
{
    mResource = resource;
    mType = type;
    mLocation = false;
    mHasDefaultObjectKeyAndId = false;
    mDefaultObjectKey = "";
    mDefaultObjectId = -1;
    mIgnoreMalformedHeaders = false;
}

GetObjectRequest::~GetObjectRequest()
{
}

bool GetObjectRequest::GetLocation() const
{
    return mLocation;
}

void GetObjectRequest::SetLocation(bool location)
{
    mLocation = location;
}

void GetObjectRequest::AddObject(string resourceEntity, int objectId)
{
    StringVectorPtr objectIds = GetObjectIds(resourceEntity);
    objectIds->push_back(lexical_cast<string>(objectId));
    UpdateDefaultObjectKeyAndId();
}

void GetObjectRequest::AddAllObjects(string resourceEntity)
{
    StringVectorPtr objectIds = GetObjectIds(resourceEntity);
    objectIds->push_back("*");
    UpdateDefaultObjectKeyAndId();
}

StringVectorPtr GetObjectRequest::GetObjectIds(string resourceEntity)
{
    ObjectList::iterator i = mObjectList.find(resourceEntity);
    if (i != mObjectList.end())
    {
        return i->second;
    }
    else
    {
        StringVectorPtr objectIds(new StringVector());
        mObjectList[resourceEntity] = objectIds;
        return objectIds;
    }
}

RetsHttpRequestPtr GetObjectRequest::CreateHttpRequest() const
{
    RetsHttpRequestPtr httpRequest(new RetsHttpRequest());
    PrepareHttpRequest(httpRequest);
    return httpRequest;
}

void GetObjectRequest::PrepareHttpRequest(RetsHttpRequestPtr httpRequest) const
{
    httpRequest->SetQueryParameter("Resource", mResource);
    httpRequest->SetQueryParameter("Type", mType);
    httpRequest->SetQueryParameter("Location", mLocation ? "1" : "0");
    httpRequest->SetMethod(RetsHttpRequest::GET);
    StringVector ids;
    ObjectList::const_iterator i;
    for (i = mObjectList.begin(); i != mObjectList.end(); i++)
    {
        string id;
        id.append(i->first);
        id.append(":");
        StringVectorPtr objectIds = i->second;
        id.append(join(*objectIds, ":"));
        ids.push_back(id);
    }
    httpRequest->SetQueryParameter("ID", join(ids, ","));
}

void GetObjectRequest::UpdateDefaultObjectKeyAndId()
{
    if (mObjectList.size() == 1)
    {
        ObjectList::const_iterator i = mObjectList.begin();
        StringVectorPtr objectIds = i->second;
        if (objectIds->size() == 1)
        {
            string objectId = objectIds->at(0);
            if (objectId !=  "*")
            {
                mHasDefaultObjectKeyAndId = true;
                mDefaultObjectKey = i->first;
                mDefaultObjectId = lexical_cast<int>(objectId);
                return;
            }
        }
    }
    mHasDefaultObjectKeyAndId = false;
    mDefaultObjectKey = "";
    mDefaultObjectId = -1;
}

bool GetObjectRequest::HasDefaultObjectKeyAndId() const
{
    return mHasDefaultObjectKeyAndId;
}

string GetObjectRequest::GetDefaultObjectKey() const
{
    return mDefaultObjectKey;
}

int GetObjectRequest::GetDefaultObjectId() const
{
    return mDefaultObjectId;
}

bool GetObjectRequest::GetIgnoreMalformedHeaders() const
{
    return mIgnoreMalformedHeaders;
}

void GetObjectRequest::SetIgnoreMalformedHeaders(bool ignore)
{
    mIgnoreMalformedHeaders = ignore;
}

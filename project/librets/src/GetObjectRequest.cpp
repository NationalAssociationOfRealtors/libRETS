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

GetObjectRequest::GetObjectRequest(string resource, string type)
{
    mResource = resource;
    mType = type;
    mLocation = false;
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
}

void GetObjectRequest::AddAllObjects(string resourceEntity)
{
    StringVectorPtr objectIds = GetObjectIds(resourceEntity);
    objectIds->push_back("*");
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


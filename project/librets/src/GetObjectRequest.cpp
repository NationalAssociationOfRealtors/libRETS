#include <iostream>
#include "librets/GetObjectRequest.h"
#include "librets/RetsHttpRequest.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;

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
    objectIds->push_back(stringValue(objectId));
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


#include <iostream>
#include "librets/GetObjectQuery.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostream;

GetObjectQuery::GetObjectQuery()
{
    mObjectIds.reset(new IntVector());
}

string GetObjectQuery::GetResource() const
{
    return mResource;
}

void GetObjectQuery::SetResource(string resource)
{
    mResource = resource;
}

string GetObjectQuery::GetType() const
{
    return mType;
}

void GetObjectQuery::SetType(string type)
{
    mType = type;
}

string GetObjectQuery::GetObjectKey() const
{
    return mObjectKey;
}

void GetObjectQuery::SetObjectKey(string objectKey)
{
    mObjectKey = objectKey;
}

IntVectorPtr GetObjectQuery::GetObjectIds() const
{
    return mObjectIds;
}

void GetObjectQuery::AddObjectId(int objectId)
{
    mObjectIds->push_back(objectId);
}

ostream & GetObjectQuery::Print(ostream & outputStream) const
{
    return outputStream
        << "Type <" << mType << ">, Object key <" << mObjectKey << ">";
}

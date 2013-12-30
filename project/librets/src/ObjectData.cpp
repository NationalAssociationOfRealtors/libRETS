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

#include <sstream>
#include <boost/algorithm/string.hpp>
#include "librets/ObjectData.h"
#include "librets/util.h"
#include "librets/RetsException.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::istringstream;
using std::ostringstream;
namespace b = boost;
namespace ba = boost::algorithm;

ObjectData::ObjectData()
{
    mObjectData.clear();
}

ObjectData::~ObjectData()
{
}

void ObjectData::Parse(string objectData)
{
    string          key;
    string          value;

    if (!splitField(objectData, "=", key, value))
    {
        return;
    }

    ba::trim(key);
    ba::to_lower(key);
    ba::trim(value);

    mObjectData[key] = value;
}

string ObjectData::GetUID() const
{
    return GetValue("UID");
}

string ObjectData::GetObjectType() const
{
    return GetValue("ObjectType");
}

string ObjectData::GetResourceName() const
{
    return GetValue("ResourceName");
}

string  ObjectData::GetResourceID() const
{
    return GetValue("ResourceID");
}

string ObjectData::GetObjectID() const
{
    return GetValue("ObjectID");
}

string ObjectData::GetMimeType() const
{
    return GetValue("MimeType");
}

bool ObjectData::GetIsDefault() const
{
    return GetValue("IsDefault") == "1";
}

string ObjectData::GetObjectModificationTimestamp() const
{
    return GetValue("ObjectModificationTimestamp");
}

string ObjectData::GetModificationTimestamp() const
{
    return GetValue("ModificationTimestamp");
}

string ObjectData::GetOrderHint() const
{
    return GetValue("OrderHint");
}

string ObjectData::GetDescription() const
{
    return GetValue("Description");
}

string ObjectData::GetCaption() const
{
    return GetValue("Caption");
}

string ObjectData::GetFileSize() const
{
    return GetValue("FileSize");
}

string ObjectData::GetWidthPix() const
{
    return GetValue("WidthPix");
}

string ObjectData::GetHeightPix() const
{
    return GetValue("HeightPix");
}

string ObjectData::GetDuration() const
{
    return GetValue("Duration");
}

string ObjectData::GetWidthInch() const
{
    return GetValue("WidthInch");
}

string ObjectData::GetHeightInch() const
{
    return GetValue("HeightInch");
}

string ObjectData::GetValue(string key) const
{
    StringMap::const_iterator value = mObjectData.find(ba::to_lower_copy(key));
    if (value == mObjectData.end())
    {
        return "";
    }
    return value->second;
}

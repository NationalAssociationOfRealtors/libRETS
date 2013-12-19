/*
 * Copyright (C) 2005-2013 National Association of REALTORS(R)
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
#include "librets/SessionInfo.h"
#include "librets/util.h"
#include "librets/RetsException.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::istringstream;
using std::ostringstream;
namespace b = boost;
namespace ba = boost::algorithm;

SessionInfo::SessionInfo()
{
}

SessionInfo::~SessionInfo()
{
}

void SessionInfo::ParseInfo(string info)
{
    string          key;
    StringVector    line;
    string          type;
    string          value;

    mSessionInformationTokens.append(info);
    mSessionInformationTokens.append("\r\n");
    
    ba::split(line, info, ba::is_any_of(";"));

    if (line.empty())
    {
        return;
    }
    
    if (line.size() >= 1)
    {
        key     = line.at(0);        
    }
    if (line.size() >= 2)
    {
        type    = line.at(1);
        if (type.length() == 0)
        {
            type = "Character";
        }
    }
    if (line.size() >= 3)
    {
        value   = line.at(2);        
    }

    ba::trim(key);
    ba::to_upper(key);
    ba::trim(type);
    ba::trim(value);

    mTypes[key] = type;
    mValues[key] = value;
}

string SessionInfo::GetType(string key) const
{
    ba::to_upper(key);
    StringMap::const_iterator value = mTypes.find(key);
    if (value == mTypes.end())
    {
        return "Character";
    }
    return value->second;
}

string SessionInfo::GetValue(string key) const
{
    ba::to_upper(key);
    StringMap::const_iterator value = mValues.find(key);
    if (value == mValues.end())
    {
        return "";
    }
    return value->second;
}

string SessionInfo::GetSessionInformationTokens() const
{
    return mSessionInformationTokens;
}

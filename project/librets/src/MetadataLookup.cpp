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

#include "librets/MetadataLookup.h"

using namespace librets;
using std::string;

#define CLASS MetadataLookup

MetadataElement::MetadataType CLASS::GetType() const
{
    return LOOKUP;
}

string CLASS::GetId() const
{
    return GetLookupName();
}

string CLASS::GetLookupName() const
{
    return GetStringAttribute("LookupName");
}

string CLASS::GetVisibleName() const
{
    return GetStringAttribute("VisibleName");
}

string CLASS::GetVersion() const
{
    /*
     * RETS 1.7.2 DTD shows "LookupTypeVersion", but the spec
     * shows "Version". Check for both. This will also handle
     * backwards compatibility with 1.5 servers.
     */
    string version = GetStringAttribute("LookupTypeVersion");
    if (version.length() == 0)
        version = GetStringAttribute("Version");
    
    return version;
}

string CLASS::GetDate() const
{
    /*
     * RETS 1.7.2 DTD shows "LookupTypeDate", but the spec
     * shows "Date". Check for both.
     */    
    string date = GetStringAttribute("LookupTypeDate");
    if (date.length() == 0)
        date = GetStringAttribute("Date");
    
    return date;
}

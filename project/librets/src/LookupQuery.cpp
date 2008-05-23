/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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

#include "librets/LookupQuery.h"

using namespace librets;
using std::string;

LookupQuery::LookupQuery() { }
LookupQuery::~LookupQuery() { }

string LookupQuery::GetResource() const
{
    return mResource;
}

void LookupQuery::SetResource(std::string resource)
{
    mResource = resource;
}

string LookupQuery::GetLookup() const
{
    return mLookup;
}

void LookupQuery::SetLookup(std::string lookup)
{
    mLookup = lookup;
}

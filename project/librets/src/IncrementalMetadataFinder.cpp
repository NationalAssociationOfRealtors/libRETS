/*
 * Copyright (C) 2005-2009 National Association of REALTORS(R)
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

#include "librets/IncrementalMetadataFinder.h"
#include "librets/DefaultMetadataCollector.h"
#include "librets/str_stream.h"

using namespace librets;
using std::string;

#define CLASS IncrementalMetadataFinder

CLASS::CLASS(MetadataLoader * loader)
  : mLoader(loader), mLoadedMetadata(new DefaultMetadataCollector())
{
    mLoader->SetCollector(mLoadedMetadata);
}

string CLASS::KeyForCache(MetadataElement::Type type, string level) const
{
    return str_stream() << type << " " << level;
}

void CLASS::EnsureLevelIsLoaded(MetadataElement::Type type, string level) const
{
    string key = KeyForCache(type, level);
    
    TypeLevelCache::const_iterator i;
    i = mTypeLevelCache.find(key);
    if (i == mTypeLevelCache.end())
    {
        mLoader->LoadMetadata(type, level);
        mTypeLevelCache.insert(key);
    }
}

MetadataElementListPtr CLASS::FindByLevel(MetadataElement::Type type,
                                          string level)
{
    EnsureLevelIsLoaded(type, level);
    return mLoadedMetadata->FindByLevel(type, level);
}

MetadataElementPtr CLASS::FindByPath(MetadataElement::Type type,
                                     string level, string id)
{
    EnsureLevelIsLoaded(type, level);
    return mLoadedMetadata->FindByPath(type, level, id);
}

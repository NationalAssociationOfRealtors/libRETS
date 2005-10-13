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

#ifndef LIBRETS_INCREMENTAL_METADATA_FINDER_H
#define LIBRETS_INCREMENTAL_METADATA_FINDER_H

#include <set>
#include "librets/metadata_forward.h"
#include "librets/MetadataFinder.h"
#include "librets/MetadataLoader.h"

namespace librets {
    
class IncrementalMetadataFinder : public RetsObject,
    public MetadataFinder
{
  public:
    IncrementalMetadataFinder(MetadataLoader * loader);
    
    virtual MetadataElementListPtr FindByLevel(MetadataElement::Type type,
                                               std::string level);
    
    virtual MetadataElementPtr FindByPath(MetadataElement::Type type,
                                          std::string level,
                                          std::string id);
    
  private:
    typedef std::set<std::string> TypeLevelCache;
    
    std::string KeyForCache(MetadataElement::Type type,
                            std::string level) const;
    
    void EnsureLevelIsLoaded(MetadataElement::Type type,
                             std::string level) const;

    MetadataLoader * mLoader;
    DefaultMetadataCollectorPtr mLoadedMetadata;
    mutable TypeLevelCache mTypeLevelCache;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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

#ifndef LIBRETS_TEST_METADATA_LOADER_H
#define LIBRETS_TEST_METADATA_LOADER_H

#include "librets/RetsObject.h"
#include "librets/MetadataLoader.h"

namespace librets {
    
class TestMetadataLoader : public RetsObject, public MetadataLoader
{
  public:
    TestMetadataLoader();
    
    void SetCollector(MetadataElementCollectorPtr collector);

    void LoadMetadata(MetadataElement::Type type, std::string level);
    
    int GetLoadCount(MetadataElement::Type type);
        
    int GetTotalLoadCount();
    
    void AddElement(MetadataElementPtr element);
    
  private:
    typedef std::map<MetadataElement::Type, int> LoadCounter;
    
    MetadataByLevelCollectorPtr mPrivateCollector;
    MetadataElementCollectorPtr mCollector;
    LoadCounter mLoadCounter;
    int mTotalLoadCount;
};

typedef boost::shared_ptr<TestMetadataLoader> TestMetadataLoaderPtr;

}

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

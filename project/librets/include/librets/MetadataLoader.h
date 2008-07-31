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

#ifndef LIBRETS_METADATA_LOADER_H
#define LIBRETS_METADATA_LOADER_H
/**
 * @file MetadataLoader.h
 * (Internal) Contains the MetadataLoader class definition.
 */
/// @cond MAINTAINER

#include "librets/metadata_forward.h"
#include "librets/MetadataElement.h"

namespace librets {
    
/**
 * (Internal) MetadataLoader defines the API that loads the metadata tree
 */    
class MetadataLoader
{
  public:
    virtual ~MetadataLoader();
    
    /**
     * Set the method that will collect the metadata.
     * @param collector A pointer to the MetadataElementCollector that will
     * do the actual loading.
     */
    virtual void SetCollector(MetadataElementCollectorPtr collector) = 0;
    
    /**
     * Load the metadata for the level and resource/class.
     * @param type The metadata level such as SYSTEM or TABLE.
     * @param level A string representing the Resource/Class to load.
     */
    virtual void LoadMetadata(MetadataElement::Type type,
                              std::string level) = 0;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

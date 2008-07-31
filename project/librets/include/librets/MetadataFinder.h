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

#ifndef LIBRETS_METADATA_FINDER_H
#define LIBRETS_METADATA_FINDER_H
/**
 * @file MetadataFinder.h
 * (Internal) Contains the MetadataFinder class definition.
 */
/// @cond MAINTAINER

#include "librets/metadata_forward.h"
#include "librets/MetadataElement.h"

namespace librets {

/**
 * (Internal) MetadataFinder defines the API by which other classes that implement
 * the actual metadata finding process inherit.
 */    
class MetadataFinder
{
  public:
    virtual ~MetadataFinder();

    /**
     * Find the metadta for a given table/object.
     * @param type The type of the metadata such as TABLE, SYSTEM, RESOURCE.
     * @param level The resource/class of the metadata such as "Property:RES".
     * @return A pointer to the MetadataElementList containing the metadata.
     */
    virtual MetadataElementListPtr FindByLevel(MetadataElement::Type type,
                                               std::string level) = 0;
    /**
     * Find the metadata for a given table/object by Id.
     * @param type The type of the metadata such as TABLE, SYSTEM, RESOURCE.
     * @param level The resource/class of the metadata such as "Property:RES".
     * @param id The Id of the specific metadata element to retrieve.
     * @return A pointer to the MetadataElement containing the metadata.
     */
    virtual MetadataElementPtr FindByPath(MetadataElement::Type type,
                                          std::string level,
                                          std::string id) = 0;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

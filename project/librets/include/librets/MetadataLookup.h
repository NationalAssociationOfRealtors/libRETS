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
#ifndef LIBRETS_METADATA_LOOKUP_H
#define LIBRETS_METADATA_LOOKUP_H

#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataLookup objects. */
typedef std::vector<MetadataLookupTypePtr> MetadataLookupTypeList;
/** A smart pointer to MetadataLookupType. */
typedef boost::shared_ptr<MetadataLookupTypeList> MetadataLookupTypeListPtr;

/**
 * Lookup metadata.
 */
class MetadataLookup : public MetadataElement
{
  public:
    /**
     * Always returns LOOKUP.
     *
     * @return LOOKUP
     */
    virtual MetadataType GetType() const;

    /**
     * Returns all lookup types.
     *
     * @return All lookup types
     */
    MetadataLookupTypeListPtr GetLookupTypes() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

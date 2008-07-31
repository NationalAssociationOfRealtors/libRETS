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
#ifndef LIBRETS_METADATA_VALIDATION_LOOKUP_H
#define LIBRETS_METADATA_VALIDATION_LOOKUP_H
/** 
 * @file MetadataValidationLookup.h
 * Contains the MetadataValidationLookup class declaration.
 */
#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataValidationLookupType objects. */
typedef std::vector<MetadataValidationLookupTypePtr>
    MetadataValidationLookupTypeList;
/** A smart pointer to MetadataValidationLookupTypeList. */
typedef boost::shared_ptr<MetadataValidationLookupTypeList>
    MetadataValidationLookupTypeListPtr;

/**
 * MetadataValidationLookup is an implementation of MetadataElement that represents
 * Validation Lookup metadata.
 */
class MetadataValidationLookup : public MetadataElement
{
  public:
    /**
     * Always returns VALIDATION_LOOKUP.
     *
     * @return VALIDATION_LOOKUP
     */
    virtual MetadataType GetType() const;

    /**
     * Returns all validation lookup types.
     *
     * @return All validation lookup types
     */
    MetadataValidationLookupTypeListPtr GetValidationLookupTypes() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

/*
 * Copyright (C) 2014 Real Estate Standards Organziation
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
#ifndef LIBRETS_METADATA_COLUMN_GROUP_NORMALIZATION_H
#define LIBRETS_METADATA_COLUMN_GROUP_NORMALIZATION_H
/** 
 * @file MetadataColumnGroupNormalization.h
 * Contains the MetadataColumnGroupNormalization class declaration.
 */

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * RETS 1.8
 * MetadataColumnGroupNormalization is an implementation of MetadataElement that
 * represents ColumnGroupSet metadata.
 */
class MetadataColumnGroupNormalization : public MetadataElement
{
  public:
    /**
     * Always returns COLUMN_GROUP_NORMALIZATION.
     *
     * @return COLUMN_GROUP_NORMALIZATION
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;

    /**
     * Returns the Type Identifier
     *
     * @return The Type Identifier
     */
    std::string GetTypeIdentifier() const;

    /**
     * Returns the Sequence of this row
     *
     * @returns The Sequence of this row
     */
    int GetSequence() const;

    /**
     * Returns the X-Axis Column Label
     * @return A string representing the Label
     */
    std::string GetColumnLabel() const;
    
    /**
     * Returns the SystemName
     *
     * @return The SystemName
     */
    std::string GetSystemName() const;    
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

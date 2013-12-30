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
#ifndef LIBRETS_METADATA_FILTER_TYPE_H
#define LIBRETS_METADATA_FILTER_TYPE_H
/** 
 * @file MetadataFilter.h
 * Contains the MetadataFilter class declaration.
 */

#include <string>
#include "librets/MetadataElement.h"

namespace librets {

/**
 * RETS 1.8
 * MetadataFilterType is an implementation of MetadataElement that
 * represents Filter Type information for the metadata
 */
class MetadataFilterType : public MetadataElement
{
  public:
    virtual ~MetadataFilterType();
    
    /**
     * Always returns FILTER_TYPE.
     *
     * @return FILTER_TYPE
     */
    virtual MetadataType GetType() const;
    
    /**
     * Return the Filter Type ID.
     *
     * @return a string containing the Filter ID.
     */
    std::string GetID() const;
     
    /**
     * Return the Filter ID.
     *
     * @return a string containing the Filter ID.
     */
    std::string GetFilterTypeID() const;

    /**
     * Return the Parent Value.
     *
     * @return a string containing the Parent Value.
     */
    std::string GetParentValue() const;
      
    /**
     * Return the Child Value.
     *
     * @return a string containing the Child Value
     */
    std::string GetChildValue() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

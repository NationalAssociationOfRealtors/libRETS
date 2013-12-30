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
#ifndef LIBRETS_METADATA_FILTER_H
#define LIBRETS_METADATA_FILTER_H
/** 
 * @file MetadataFilter.h
 * Contains the MetadataFilter class declaration.
 */

#include <string>
#include "librets/MetadataElement.h"

namespace librets {

/**
 * RETS 1.8
 * MetadataFilter is an implementation of MetadataElement that
 * represents Filter information for the metadata
 */
class MetadataFilter : public MetadataElement
{
  public:
    virtual ~MetadataFilter();
    
    /**
     * Always returns FILTER.
     *
     * @return FILTER
     */
    virtual MetadataType GetType() const;
    
    /**
     * Return the Filter ID.
     *
     * @return a string containing the Filter ID.
     */
    std::string GetID() const;
     
    /**
     * Return the Filter ID.
     *
     * @return a string containing the Filter ID.
     */
    std::string GetFilterID() const;

    /**
     * Return the Parent Resource ID.
     *
     * @return a string containing the Parent Resource ID.
     */
    std::string GetParentResourceID() const;
      
    /**
     * Return the Parent Lookup Name.
     *
     * @return a string containing the Parent Lookup Name.
     */
    std::string GetParentLookupName() const;
      
    /**
     * Return the Child Resource ID.
     *
     * @return a string containing the Child Resource ID.
     */
    std::string GetChildResourceID() const;
     
    /**
     * Return the Child Lookup Name.
     * 
     * @return a string containing the Child Lookup Name.
     */
    std::string GetChildLookupName() const;

    /**
     * Return the Not Shown by Default flag.
     *
     * @return a boolean that if <code>true</code> indicates the
     * server will not include the FilterValue data unless specifically
     * asked using the LookupFilter argument.
     */
     bool GetNotShownByDefault() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

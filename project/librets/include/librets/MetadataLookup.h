/*
 * Copyright (C) 2005-2013 National Association of REALTORS(R)
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
/** 
 * @file MetadataLookup.h
 * Contains the MetadataLookup class declaration.
 */

#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataLookup is an implementation of MetadataElement that
 * represents Lookup metadata.
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
    
    virtual std::string GetId() const;
    /**
     * Return the Lookup Name.
     * @return A string containing the Lookup Name.
     */
    std::string GetLookupName() const;
    
    /**
     * Return the visible name of the Lookup.
     * @return A string containing the Visible Name.
     */
    std::string GetVisibleName() const;
    
    /**
     * Return the version of this lookup.
     * @return A string containing the metadata version for this lookup.
     */
    std::string GetVersion() const;
    
    /**
     * Return the metadata data associated with this lookup.
     * @return A string containing the date.
     */
    std::string GetDate() const;
    
    /**
     * RETS 1.8
     * Return the FilterID for filtering LookupType values.
     * @return A string containing the FilterID.
     */
    std::string GetFilterID() const;
    
    /**
     * RETS 1.8
     * Return a flag indicating whether or not the server will
     * automatically show data for this type unless sepcifically
     * asked for it.
     * @return A boolean indicating that the data is not shown
     * by default.
     */
    bool GetNotShownByDefault() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

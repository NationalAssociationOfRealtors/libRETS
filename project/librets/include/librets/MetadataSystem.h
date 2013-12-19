/*
 * Copyright (C) 2005-2009 National Association of REALTORS(R)
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
#ifndef LIBRETS_METADATA_SYSTEM_H
#define LIBRETS_METADATA_SYSTEM_H
/** 
 * @file MetadataSystem.h
 * Contains the MetadataSystem class declaration.
 */

#include <string>
#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataSystem is an implementation of MetadataElement that
 * represents System metadata.
 */
class MetadataSystem : public MetadataElement
{
  public:
    virtual ~MetadataSystem();

    /**
     * Returns SYSTEM.
     */
    virtual MetadataType GetType() const;

    /**
     * Returns the system ID.
     *
     * @return The system ID
     */
    std::string GetSystemID() const;

    /**
     * Returns the system description.
     *
     * @return The system description
     */
    std::string GetSystemDescription() const;

    /**
     * Returns the comments.
     *
     * @return The comments
     */
    std::string GetComments() const;
    
    /**
     * Returns the TimeZoneOffset for RETS 1.7 and later.
     *
     * @return A String containing the TimeZoneOffset.
     */
    std::string GetTimeZoneOffset() const;
    
    /**
     * RETS 1.8
     * Returns the MetadataID
     *
     * @return A String containing the MetadataID
     */
    std::string GetMetadataID() const;
    
    /**
     * RETS 1.8
     * Returns the ResourceVersion
     *
     * @return A String containing the ResourceVersion
     */
    std::string GetResourceVersion() const;
        
    /**
     * RETS 1.8
     * Returns the ResourceDate
     *
     * @return A String containing the ResourceDate
     */
    std::string GetResourceDate() const;
    
    /**
     * RETS 1.8
     * Returns the ForeignKeyVersion
     *
     * @return A String containing the ForeignKeyVersion
     */
    std::string GetForeignKeyVersion() const;
    
    /**
     * RETS 1.8
     * Returns the ForeignKeyDate
     *
     * @return A String containing the ForeignKeyDate
     */
    std::string GetForeignKeyDate() const;
    
    /**
     * RETS 1.8
     * Returns the FilterVersion
     *
     * @return A String containing the FilterVersion
     */
    std::string GetFilterVersion() const;
    
    /**
     * RETS 1.8
     * Returns the FilterDate
     *
     * @return A String containing the FilterDate
     */
    std::string GetFilterDate() const;
    
    
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

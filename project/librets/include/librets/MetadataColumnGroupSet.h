/*
 * Copyright (C) 2013 National Association of REALTORS(R)
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
#ifndef LIBRETS_METADATA_COLUMN_GROUP_SET_H
#define LIBRETS_METADATA_COLUMN_GROUP_SET_H
/** 
 * @file MetadataColumnGroupSet.h
 * Contains the MetadataColumnGroupSet class declaration.
 */

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * RETS 1.8
 * MetadataColumnGroupSet is an implementation of MetadataElement that
 * represents ColumnGroupSet metadata.
 */
class MetadataColumnGroupSet : public MetadataElement
{
  public:
    /**
     * Data types for PresntationStyle.
     */
    enum PresentationStyle
    {
        /** Basic Edit Block displayed in PresentationColumns number of columns */
        EDIT,

        /** Expected to be displayed using Normalization Grid */
        MATRIX,

        /** Show on record per row. */
        LIST,

        /** Show one record per row and allow the records to be added, edited and deleted. */
        EDIT_LIST,

        /** Expecting Lattitude and Longitude to be provided from GIS search. */
        GIS_MAP_SEARCH,

        /** Go to the specified URL. */
        URL,
        
        /** No presentation style specified */
        NO_PRESENTATION
    };
    
    /**
     * Always returns COLUMN_GROUP_SET.
     *
     * @return COLUMN_GROUP_SET
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;

    /**
     * Returns the Column Group Set within a Class
     *
     * @return The Column Group Set within a Class
     */
    std::string GetColumnGroupSetName() const;

    /**
     * Returns the Column Group Set Parent
     *
     * @return The Column Group Set Parent
     */
    std::string GetColumnGroupSetParent() const;

    /**
     * Returns the Sequence of this Column Group
     *
     * @returns The Sequence of this Column Group
     */
    int GetSequence() const;
    
    /**
     * Returns the Long name for this Column Group
     * @return A string representing the Long name for this Column Group
     */
    std::string GetLongName() const;
    
    /**
     * Return the short name for this Column Group.
     * @return A string representing the short name.
     */
    std::string GetShortName() const;
    
    /**
     * Return the Description
     * @return A string representing the Description.
     */
    std::string GetDescription() const;

    /**
     * Returns the Column Group Name
     *
     * @return The Column Group Name
     */
    std::string GetColumnGroupName() const;

    /**
     * Returns the Presentation Style
     *
     * @return The Presentation Style.
     */
    PresentationStyle GetPresentationStyle() const;

    /**
     * Returns a string containing the URL to reference instead of the
     * standard Column Group
     *
     * @return a string containing the URL
     */
    std::string GetURL() const;

    /**
     * Returns the Foreign Key ID
     * @return A string containing the Foreign Key ID
     */
    std::string GetForeignKeyID() const;
    
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

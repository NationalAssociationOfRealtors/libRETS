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
#ifndef LIBRETS_METADATA_COLUMN_GROUP_TABLE_H
#define LIBRETS_METADATA_COLUMN_GROUP_TABLE_H
/** 
 * @file MetadataColumnGroupTable.h
 * Contains the MetadataColumnGroupTable class declaration.
 */

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * RETS 1.8
 * MetadataColumnGroupTable is an implementation of MetadataElement that
 * represents ColumnGroupSet metadata.
 */
class MetadataColumnGroupTable : public MetadataElement
{
  public:
    /**
     * Always returns COLUMN_GROUP_TABLE.
     *
     * @return COLUMN_GROUP_TABLE
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;

    /**
     * Returns the SystemName
     *
     * @return The SystemName
     */
    std::string GetSystemName() const;

    /**
     * Returns the Column Group Set Name
     *
     * @return The Column Group Set Name
     */
    std::string GetColumnGroupSetName() const;

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
     * Return the Display Order
     * @return An int representing the Display Order.
     */
    int GetDisplayOrder() const;

    /**
     * Returns the Display Length
     *
     * @return An int representing the Display Length
     */
    int GetDisplayLength() const;

    /**
     * Return the Display Height
     * @return An int representing the Display Height.
     */
    int GetDisplayHeight() const;
    
    /**
     * Returns the Immediate Refresh Flag
     *
     * @return A boolean that if <code>true</code> indicates that a user change
     * to this field should result in an automatic GUI refresh.
     */
    bool GetImmediateRefresh() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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
#ifndef LIBRETS_METADATA_COLUMN_GROUP_H
#define LIBRETS_METADATA_COLUMN_GROUP_H
/** 
 * @file MetadataColumnGroup.h
 * Contains the MetadataColumnGroup class declaration.
 */

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * RETS 1.8
 * MetadataColumnGroup is an implementation of MetadataElement that
 * represents ColumnGroupSet metadata.
 */
class MetadataColumnGroup : public MetadataElement
{
  public:
    /**
     * Always returns COLUMN_GROUP_SET.
     *
     * @return COLUMN_GROUP_SET
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;

    /**
     * Returns the Column Group within a Class
     *
     * @return The Column Group within a Class
     */
    std::string GetColumnGroupName() const;

    /**
     * Returns the Control System Name
     *
     * @return The Control System Name
     */
    std::string GetControlSystemName() const;
    
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
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

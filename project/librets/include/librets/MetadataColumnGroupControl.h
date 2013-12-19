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
#ifndef LIBRETS_METADATA_COLUMN_GROUP_CONTROL_H
#define LIBRETS_METADATA_COLUMN_GROUP_CONTROL_H
/** 
 * @file MetadataColumnGroupControl.h
 * Contains the MetadataColumnGroupControl class declaration.
 */

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * RETS 1.8
 * MetadataColumnGroupControl is an implementation of MetadataElement that
 * represents ColumnGroupSet metadata.
 */
class MetadataColumnGroupControl : public MetadataElement
{
  public:
    /**
     * Always returns COLUMN_GROUP_CONTROL.
     *
     * @return COLUMN_GROUP_CONTROL
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;

    /**
     * Returns the minimum value the the ControlSystemName is allowed to have.
     *
     * @return The minimum value the the ControlSystemName is allowed to have.
     */
    int GetLowValue() const;

    /**
     * Returns the maximum value the the ControlSystemName is allowed to have.
     *
     * @return The maximum value the the ControlSystemName is allowed to have.
     */
    int GetHighValue() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

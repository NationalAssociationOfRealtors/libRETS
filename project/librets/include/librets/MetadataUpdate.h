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
#ifndef LIBRETS_METADATA_UPDATE_H
#define LIBRETS_METADATA_UPDATE_H
/** 
 * @file MetadataUpdate.h
 * Contains the MetadataUpdate class declaration.
 */

#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataUpdate is an implementation of MetadataElement that represents
 * Update metadata.
 */
class MetadataUpdate : public MetadataElement
{
  public:
    /**
     * Alwyas returns UPDATE.
     *
     * @return UPDATE
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;
    
    /**
     * Return the nature of this update type.
     * @return A string representing the nature of this update metadata.
     */
    std::string GetUpdateName() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

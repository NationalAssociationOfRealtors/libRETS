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

#ifndef LIBRETS_METADATA_EDIT_MASK_H
#define LIBRETS_METADATA_EDIT_MASK_H
/** 
 * @file MetadataEditMask.h
 * Contains the MetadataEditMask class declaration.
 */

#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataEditMask is an implementation of MetadataElement that
 * represents Edit mask metadata.
 */
class MetadataEditMask : public MetadataElement
{
  public:
    /**
     * Always returns EDIT_MASK.
     *
     * @return EDIT_MASK
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;
    /**
     * Returns the Edit Mask ID attribute.
     * @return A string containing the edit mask ID.
     */
    std::string GetEditMaskID() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

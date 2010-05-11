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
#ifndef LIBRETS_METADATA_UPDATE_TYPE_H
#define LIBRETS_METADATA_UPDATE_TYPE_H
/** 
 * @file MetadataUpdateType.h
 * Contains the MetadataUpdateType class declaration.
 */

#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataUpdateType is an implementation of MetadataElement that represents
 * Update Type metadata.
 */
class MetadataUpdateType : public MetadataElement
{
  public:
    /**
     * Always returns UPDATE_TYPE.
     *
     * @return UPDATE_TYPE
     */
    virtual MetadataType GetType() const;
    
    /**
     * Return the <code>SystemName</code> for this Update Type.
     * @return A string containing the <code>SystemName</code>.
     */
    std::string GetSystemName() const;
    
    /**
     * Return the sequence number of this field.
     * @return An integer representing the sequence number for this field.
     */
    int GetSequence() const;
    
    /**
     * Return the <code>Attributes</code> for this field.
     * @return A string containing a comma separated string of Update Attributes.
     */
    std::string GetAttributes() const;
    
    /**
     * Return the default value for this field.
     * @return A string containing the default value.
     */
    std::string GetDefault() const;
    
    /**
     * Return a string containing the <code>ValidationExpressionID</code> for this field.
     * @return A string containing the <code>ValidationExpressionID</code>.
     */
    std::string GetValidationExpressionID() const;
    
    /**
     * Return the <code>ValidationLookupName</code> for this field.
     * @return A string containing the <code>ValidationLookupName</code>.
     */
    std::string GetValidationLookupName() const;
    
    /**
     * Return the <code>ValidationExternalName</code> for this field.
     * @return A string containing the <code>ValidationExternalName</code>.
     */
    std::string GetValidationExternalName() const;
    
    /**
     * Return the maximum number of values that may be specified for the field.
     * @return An integer containing the maximum number of values.
     */
    int GetMaxUpdate() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

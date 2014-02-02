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
#ifndef LIBRETS_METADATA_CLASS_H
#define LIBRETS_METADATA_CLASS_H
/** 
 * @file MetadataClass.h
 * Contains the MetadataClass class declaration.
 */

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataClass is an implementation of MetadataElement that represents
 * Class information for the metadata.
 */
class MetadataClass : public MetadataElement
{
  public:
    /**
     * Always returns CLASS;
     *
     * @return CLASS
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;

    /**
     * Returns the name of the class metadata.
     *
     * @return Name of the class metadata.
     */
    std::string GetClassName() const;

    /**
     * Returns the standard name of the class metadata.  If no standard name
     * is given, then an empty string is returned.
     *
     * @return Standard name of the class.
     */
    std::string GetStandardName() const;

    /**
     * Returns the description of the class metadata.  If no description is
     * given, then an empty string is returned.
     *
     * @return Description of this class.
     */
    std::string GetDescription() const;

    /**
     * Returns the user-visible name of the class.
     *
     * @return Visible Name
     */
    std::string GetVisibleName() const;

    /**
     * Returns the TableVersion. RETS 1.8
     *
     * @return The TableVersion
     */
    std::string GetTableVersion() const;

    /**
     * Returns the TableDate. RETS 1.8
     *
     * @return The TableDate
     */
    std::string GetTableDate() const;

    /**
     * Returns the UpdateVersion. RETS 1.8
     *
     * @return The UpdateVersion
     */
    std::string GetUpdateVersion() const;

    /**
     * Returns the UpdateDate. RETS 1.8
     *
     * @return The UpdateDate.
     */
    std::string GetUpdateDate() const;

    /**
     * Returns the ClassTimeStamp. RETS 1.8
     *
     * @return The ClassTimeStamp
     */
    std::string GetClassTimeStamp() const;

    /**
     * Returns the Deleted Flag Field. RETS 1.8
     *
     * @return The DeletedFlagField
     */
    std::string GetDeletedFlagField() const;

    /**
     * Returns the Deleted Flag Value. RETS 1.8
     *
     * @return The DeletedFlagValue
     */
    std::string GetDeletedFlagValue() const;

    /**
     * Returns the HasKeyIndex Flag. RETS 1.8
     * 
     * @return The HasKeyIndex boolean.
     */
    bool HasKeyIndex() const;

    /**
     * Returns the OffsetSupport Flag. RETS 1.8
     *
     * @return The OffsetSupport boolean.
     */
    bool HasOffsetSupport() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

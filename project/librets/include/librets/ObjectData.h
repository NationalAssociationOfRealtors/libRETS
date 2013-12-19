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
#ifndef LIBRETS_OBJECT_DATA_H
#define LIBRETS_OBJECT_DATA_H
/**
 * @file ObjectData.h 
 * Contains the ObjectData class definition.
 */
 
#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/EncodingType.h"
#include "librets/RetsObject.h"
#include "librets/RetsVersion.h"

namespace librets {

/**
 * RETS 1.8
 * ObjectData is a class that parses a string in the form of 
 * <code>ObjectData: RETSNAME=TEXT</code>, 
 * saves and enables access to the results.
 */
class ObjectData : public RetsObject
{
  public:
    /**
     * Default constructor.
     */
    ObjectData();
    
    virtual ~ObjectData();

    /**
     * Return the value for the well know name UID
     * @return A string containing the UID
     */
    std::string GetUID() const;

    /**
     * Return the value for the well know name ObjectType
     * @return A string containing the ObjectType
     */
    std::string GetObjectType() const;

    /**
     * Return the value for the well know name ResourceName
     * @return A string containing the ResourceName
     */
    std::string GetResourceName() const;

    /**
     * Return the value for the well know name ResourceID
     * @return A string containing the ResourceID
     */
    std::string GetResourceID() const;

    /**
     * Return the value for the well know name ObjectID
     * @return A string containing the ObjectID
     */
    std::string GetObjectID() const;

    /**
     * Return the value for the well know name MimeType
     * @return A string containing the MimeType
     */
    std::string GetMimeType() const;

    /**
     * Return the value for the well know name IsDefault
     * @return A bool indicating if this is the default object
     */
    bool GetIsDefault() const;

    /**
     * Return the value for the well know name ObjectModificationTimestamp
     * @return A string containing the ObjectModificationTimestamp
     */
    std::string GetObjectModificationTimestamp() const;

    /**
     * Return the value for the well know name ModificationTimestamp
     * @return A string containing the ModificationTimestamp
     */
    std::string GetModificationTimestamp() const;

    /**
     * Return the value for the well know name OrderHint
     * @return A string containing the OrderHint
     */
    std::string GetOrderHint() const;

    /**
     * Return the value for the well know name Description
     * @return A string containing the Description
     */
    std::string GetDescription() const;

    /**
     * Return the value for the well know name Caption
     * @return A string containing the Caption
     */
    std::string GetCaption() const;

    /**
     * Return the value for the well know name FileSize
     * @return A string containing the FileSize
     */
    std::string GetFileSize() const;

    /**
     * Return the value for the well know name WidthPix
     * @return A string containing the WidthPix
     */
    std::string GetWidthPix() const;

    /**
     * Return the value for the well know name HeightPix
     * @return A string containing the HeightPix
     */
    std::string GetHeightPix() const;

    /**
     * Return the value for the well know name Duration
     * @return A string containing the Duration
     */
    std::string GetDuration() const;

    /**
     * Return the value for the well know name WidthInch
     * @return A string containing the WidthInch
     */
    std::string GetWidthInch() const;

    /**
     * Return the value for the well know name HeightInch
     * @return A string containing the HeightInch
     */
    std::string GetHeightInch() const;

    /**
     * For a given key, return the associated value. If the key is not found
     * an empty string is returned.
     * @param key A string representing the key whose value is to be returned.
     * @return A string containing the value.
     */
    std::string GetValue(std::string key) const;

    /// @cond MAINTAINER
    /**
     * (Internal) Parse the line as a Object Data header.
     */
    void Parse(std::string info);
    /// @endcond

  private:
    StringMap mObjectData;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

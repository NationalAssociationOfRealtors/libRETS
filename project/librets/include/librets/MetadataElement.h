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
#ifndef LIBRETS_METADATA_ELEMENT_H
#define LIBRETS_METADATA_ELEMENT_H
/** 
 * @file MetadataElement.h
 * Contains the MetadataElement class declaration.
 */

#include <string>
#include <vector>
#include <algorithm>
#include "librets/std_forward.h"
#include "librets/metadata_forward.h"
#include "librets/RetsObject.h"

namespace librets {

/** A vector of MetadataElement objects. */
typedef std::vector<MetadataElementPtr> MetadataElementList;
/** A smart pointer to MetadataElementList. */
typedef boost::shared_ptr<MetadataElementList> MetadataElementListPtr;

/**
 * MetadataElement is the fundamental class that represents an element of 
 * metadata.
 */
class MetadataElement : public RetsObject
{
  public:

    /**
     * Metadata types.
     */
    enum MetadataType
    {
        /** System metadata. */
        SYSTEM,
        /** Resource metadata. */
        RESOURCE,
        /** Class metadata. */
        CLASS,
        /** Table metadata. */
        TABLE,
        /** Update metadata. */
        UPDATE,
        /** Update type metadata. */
        UPDATE_TYPE,
        /** Object metadata. */
        OBJECT,
        /** Search help metadata. */
        SEARCH_HELP,
        /** Edit mask metadata. */
        EDIT_MASK,
        /** Lookup metadata. */
        LOOKUP,
        /** Lookup type metadata. */
        LOOKUP_TYPE,
        /** Update help metadata. */
        UPDATE_HELP,
        /** Validation lookup metadata. */
        VALIDATION_LOOKUP,
        /** Validation lookup type metadata. */
        VALIDATION_LOOKUP_TYPE,
        /** Validation external metadata. */
        VALIDATION_EXTERNAL,
        /** Validation external type metadata. */
        VALIDATION_EXTERNAL_TYPE,
        /** Validation expression metadata. */
        VALIDATION_EXPRESSION,
        /** Foreign key metadata. */
        FOREIGN_KEY
    };
    
    typedef MetadataType Type;

    MetadataElement();

    virtual ~MetadataElement();

    /**
     * Returns the type of metadata element.
     *
     * @return the type of metadata element
     */
    virtual MetadataType GetType() const = 0;

    /**
     * Returns all attirbute names.
     *
     * @return all attribute names
     */
    StringVector GetAttributeNames() const;

    /**
     * Returns the value of an attribute.
     *
     * @param attributeName A string containing the name of the attribute.
     * @param defaultValue The value to use if the attribute does not exist.
     * @return The value of the attribute as a string.
     */
    std::string GetStringAttribute(std::string attributeName,
                                   std::string defaultValue = "") const;
    /**
     * Returns the value of an attribute as an integer.
     * @param attributeName A string containing the name of the attribute.
     * @param defaultValue The value to use if the attribute does not exist.
     * @return The value of the attribute as an integer.
     */
    int GetIntAttribute(std::string attributeName, int defaultValue = 0) const;
    
    /**
     * Returns the value of an attribute as a boolean.
     * @param attributeName A string containing the name of the attribute.
     * @param defaultValue The value to use if the attribute does not exist.
     * @return The value of the attribute as a boolean.
     */
    bool GetBoolAttribute(std::string attributeName, bool defaultValue = false)
        const;
        
    /// @cond MAINTAINER
    /**
     * Set the value of an attribute.
     * @param attributeName A string containing the name of the attribute.
     * @param attributeValue A string containing the value of the attribute.
     */
    void SetAttribute(std::string attributeName, std::string attributeValue);
    /// @endcond
    
    /**
     * Returns the ID attribute.  The ID attribute is a non-empty and unique
     * field used to identify this element within the scope of elements
     * of the same type.  Not all metadata elements have an identifying
     * attribute.  In these cases, an empty string is returned.
     * The default implementation returns an empty string.  Subclasses
     * should override as needed. 
     *
     * @return the ID attribute, if there is one
     */
    virtual std::string GetId() const;

    /**
     * Returns the level of the current metadata element.
     * @return A string containing the level.
     */
    std::string GetLevel() const;

    /// @cond MAINTAINER
    /**
     * Set the level for the current metadata element.
     * @param level A string containing the level.
     */
    void SetLevel(std::string level);
    /// @endcond
    
    /**
     * Returns the path of this element.  The path uniquely identifies this
     * metadata element amongst all elements of the same type.  Since
     * this relies on the ID attribute, only those elements with an ID
     * attribute have a non-empty path.
     *
     * @return the unique path
     */
    std::string GetPath() const;
    
    /**
     * Returns the MetadataEntryId for the element.
     *
     * @return MetadataEntryId
     */
    std::string GetMetadataEntryID() const;

    /**
     * Prints the attribute in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;

  protected:
    StringMap mAttributes;
    std::string mLevel;
};

/// @cond MAINTAINER
/**
 * MetadataElementIdEqual is a class that determines whether or not
 * the ID of two metadata elements are the same. This only appears to
 * be used in the test suite as of 1.2.1.
 */
class MetadataElementIdEqual :
    public std::unary_function<MetadataElement *, bool>
{
    std::string mId;

  public:
    MetadataElementIdEqual(std::string id) : mId(id) { }
    bool operator()(const MetadataElement * element)
    {
        return (element->GetId() == mId);
    }
    
    bool operator()(const MetadataElementPtr element)
    {
        return (*this)(element.get());
    }
};
/// @endcond
};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

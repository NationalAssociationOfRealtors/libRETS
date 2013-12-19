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
#ifndef LIBRETS_METADATA_TABLE_H
#define LIBRETS_METADATA_TABLE_H
/** 
 * @file MetadataTable.h
 * Contains the MetadataTable class declaration.
 */

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * MetadataTable is an implementation of MetadataElement that
 * represents Table metadata.
 */
class MetadataTable : public MetadataElement
{
  public:
    /**
     * Data types for tables.
     */
    enum DataType
    {
        /** A boolean stored as 1 for true and 0 for false. */
        BOOLEAN,

        /** An arbitrary sequence of characters. */
        CHARACTER,

        /** A date stored in YYYY-MM-DD format. */
        DATE,

        /** A timestamp stored in YYYY-MM-DDThh:mm:ss[.SSS] format. */
        DATE_TIME,

        /** A time stored in hh:mm:ss[.SSS] format. */
        TIME,

        /** A signed numeric value that can be stored in 8 bits. */
        TINY,

        /** A signed numeric value that can be stored in 16 bits. */
        SMALL,

        /** A signed numeric value that can be stored in 32 bits. */
        INT,

        /** A signed numeric value that can be stored in 32 bits. */
        LONG,

        /** A decimal value that contains a decimal point. */
        DECIMAL
    };
    
    enum Interpretation
    {
        NUMBER,
        CURRENCY,
        LOOKUP,
        LOOKUP_MULTI,
        LOOKUP_BITSTRING,
        LOOKUP_BITMASK,
        URI,
        NO_INTERPRETATION
    };
    
    enum Alignment
    {
        LEFT,
        RIGHT,
        CENTER,
        JUSTIFY,
        NO_ALIGNMENT
    };
    
    enum Units
    {
        FEET,
        METERS,
        SQUARE_FEET,
        SQUARE_METERS,
        ACRES,
        HECTARES,
        NO_UNITS
    };
    
    enum CharacterCase
    {
        UPPER,
        LOWER,
        EXACT,
        MIXED,
        NO_CASE
    };

    /**
     * Always returns TABLE.
     *
     * @return TABLE
     */
    virtual MetadataType GetType() const;
    
    virtual std::string GetId() const;

    /**
     * Returns the system name for this field.
     *
     * @return The system name for this field.
     */
    std::string GetSystemName() const;

    /**
     * Returns the standard name for this field.  If this table has no
     * standard name, then an empty string is returned.
     *
     * @return The standard name for this field
     */
    std::string GetStandardName() const;

    /**
     * Returns the long, descriptive name for this field.
     *
     * @returns The long name for this field
     */
    std::string GetLongName() const;
    
    /**
     * Returns the DB name for this field.
     * @return A string representing the DB name.
     */
    std::string GetDBName() const;
    
    /**
     * Return the short name for this field.
     * @return A string representing the short name.
     */
    std::string GetShortName() const;
    
    /**
     * Return the maximum length for this field.
     * @return An integer representing the maximum length
     */
    int GetMaximumLength() const;

    /**
     * Returns the data type for this field.
     *
     * @return The data type for this field
     */
    DataType GetDataType() const;

    /**
     * Returns the maximum length of this field, in characters.  For
     * numeric fields, this is the display length rather than the
     * storage length, and iuncludes all formatting such as the sign,
     * decimal point, commas or other insertion edits.
     *
     * @return The maximum length of this field, in characters
     */
    int GetPrecision() const;

    /**
     * Returns <code>true</code> if this field is searchable.
     *
     * @return <code>true</code> if this field is searchable
     */
    bool IsSearchable() const;

    /**
     * Returns the intepretation of this field.
     * @return An enum representing the interpretation of this field.
     */
    Interpretation GetInterpretation() const;
    
    /**
     * Return the alignment of this field.
     * @return An enum representing the alignment of this field.
     */
    Alignment GetAlignment() const;
    
    /**
     * Return whether or not this field uses a separator.
     * @return A boolen that if TRUE indications that a separator is used.
     */
    bool UseSeparator() const;
    
    /**
     * Return the edit mask ID for this field.
     * @return A string representing the edit mask ID.
     */
    std::string GetEditMaskId() const;
    
    /**
     * Return the lookup name for this field.
     * @return A string representing the lookup name.
     */
    std::string GetLookupName() const;
    
    /**
     * Return the Max Select value for this field.
     * @return An integer representing the maximum select value.
     */
    int GetMaxSelect() const;
    
    /**
     * Return the units type for this field.
     * @return An enum representing the units.
     */
    Units GetUnits() const;
    
    /**
     * Return the index for this field.
     * @return An integer representing the index.
     */
    int GetIndex() const;
    
    /**
     * Get the minimum value for this field.
     * @return An integer representing the minimum value.
     */
    int GetMinimum() const;
    
    /**
     * Get the minimum value for this field as a string.
     * This is a workaround to allow the ability to fetch minimum values
     * for floating point fields. It is up to the user code to handle the
     * parsing of this value.
     * @return A string represnting the minimum value of this field.
     */
    std::string GetMinimumAsString() const;
    
    /**
     * Get the maximum value for this field.
     * @return An integer representing the maximum value.
     */
    int GetMaximum() const;
    
    /**
     * Get the maximum value for this field as a string.
     * This is a workaround to allow the ability to fetch maximum values
     * for floating point fields. It is up to the user code to handle the
     * parsing of this value.
     * @return A string representing the maximum value of this field.
     */
    std::string GetMaximumAsString() const;
    
    /**
     * Get the default value for this field.
     * @return An integer represnting the default value
     */
    int GetDefault() const;
    
    /**
     * Get whether or not this is a required field for searching.
     * @return An integer representing the value.
     */
    int GetRequired() const;
    
    /**
     * Get the Search Help ID.
     * @return A string representing the Search Help ID.
     */
    std::string GetSearchHelpId() const;
    
    /**
     * Get whether or not this field is supposed to be unique.
     * @return A boolean that when TRUE indicates the field should be unique.
     */
    bool IsUnique() const;
    
    /**
     * Returns <code>true</code> if changes to this field update the class's 
     * ModTimeStamp field.
     *
     * @return <code>true</code> if changes update ModTimeStamp.
     */
    bool UpdatesModTimeStamp() const;
    
    /**
     * When nonblank, indicate that this field is normally populated via this
     * foreign key.
     *
     * @return Blank or a foriegn key.
     */
    std::string GetForeignKey() const;
    
    /**
     * Returns the <code>SystemName</code> from the child record accessed via the
     * specified foreign key.
     *
     * @return <code>SystemName</code> of the foreign key for the child.
     */
    std::string GetForeignField() const;
    
    /**
     * @deprecated
     * Returns <code>true</code> when this field may be included in a query that
     * uses the <code>Key</code> optional argument.
     *
     * @return <code>true</code> when may be used with <code>Key</code> optional argument.
     */
    bool GetKeyRetrievalQuery() const;
    
    /**
     * @deprecated
     * Returns <code>true</code> when this field may be included in the <code>Select</code>
     * list of a query that uses the option <code>Key</code> argument.
     *
     * @return <code>true</code> when may be used in <code>Select</code> list.
     */
    bool GetKeyRetrievalSelect() const;

    /**
     * Returns <code>true</code> when this field may be included in the <code>Select</code>
     * list of a query in order to suppress normal <code>Limit</code> behavior.
     *
     * @return <code>true</code> when may be used in <code>Select</code> list.
     */
    bool InKeyIndex() const;
    
    /**
     * RETS 1.8
     * Returns the FilterParentField used to restrict values to a Lookup Filter.
     *
     * @return A string containing the Filter Parent Field.
     */
    std::string GetFilterParentField() const;
    
    /**
     * RETS 1.8
     * Returns the order of fields for display in a search screen.
     *
     * @return An int containing the order.
     */
    int GetDefaultSearchOrder() const;
    
    /**
     * RETS 1.8
     * Returns the Case of the field when this is a Character field.
     *
     * @return A enum containing the case.
     */
    CharacterCase GetCase() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

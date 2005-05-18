#ifndef LIBRETS_METADATA_TABLE_H
#define LIBRETS_METADATA_TABLE_H

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Table metadata.
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

    /**
     * Always returns TABLE.
     *
     * @return TABLE
     */
    virtual MetadataType GetType() const;

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
    
    std::string GetDBName() const;
    
    std::string GetShortName() const;
    
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

    Interpretation GetInterpretation() const;
    
    Alignment GetAlignment() const;
    
    bool UseSeparator() const;
    
    std::string GetEditMaskId() const;
    
    std::string GetLookupName() const;
    
    int GetMaxSelect() const;
    
    Units GetUnits() const;
    
    int GetIndex() const;
    
    int GetMinimum() const;
    
    int GetMaximum() const;
    
    int GetDefault() const;
    
    int GetRequired() const;
    
    std::string GetSearchHelpId() const;
    
    bool IsUnique() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

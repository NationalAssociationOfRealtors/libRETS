#include "librets/MetadataTable.h"

using namespace librets;
using std::string;

MetadataElement::MetadataType MetadataTable::GetType() const
{
    return TABLE;
}

string MetadataTable::GetSystemName() const
{
    return GetStringAttribute("SystemName");
}

string MetadataTable::GetStandardName() const
{
    return GetStringAttribute("StandardName");
}

string MetadataTable::GetLongName() const
{
    return GetStringAttribute("LongName");
}

string MetadataTable::GetDBName() const
{
    return GetStringAttribute("DBName");
}

string MetadataTable::GetShortName() const
{
    return GetStringAttribute("ShortName");
}

int MetadataTable::GetMaximumLength() const
{
    return GetIntAttribute("MaximumLength");
}

MetadataTable::DataType MetadataTable::GetDataType() const
{
    string dataType = GetStringAttribute("DataType");
    if (dataType == "Boolean")
    {
        return BOOLEAN;
    }
    else if (dataType == "Character")
    {
        return CHARACTER;
    }
    else if (dataType == "Date")
    {
        return DATE;
    }
    else if (dataType == "DateTime")
    {
        return DATE_TIME;
    }
    else if (dataType == "Time")
    {
        return TIME;
    }
    else if (dataType == "Tiny")
    {
        return TINY;
    }
    else if (dataType == "Small")
    {
        return SMALL;
    }
    else if (dataType == "Int")
    {
        return INT;
    }
    else if (dataType == "Long")
    {
        return LONG;
    }
    else if (dataType == "Decimal")
    {
        return DECIMAL;
    }
    else
    {
        return CHARACTER;
    }
}

int MetadataTable::GetPrecision() const
{
    return GetIntAttribute("Precision");
}

bool MetadataTable::IsSearchable() const
{
    return GetBoolAttribute("Searchable");
}

MetadataTable::Interpretation MetadataTable::GetInterpretation() const
{
    string interpretation = GetStringAttribute("Interpretation");
    if (interpretation == "Number")
    {
        return NUMBER;
    }
    else if (interpretation == "Currency")
    {
        return CURRENCY;
    }
    else if (interpretation == "Lookup")
    {
        return LOOKUP;
    }
    else if (interpretation == "LookupMulti")
    {
        return LOOKUP_MULTI;
    }
    else if (interpretation == "LookupBitstring")
    {
        return LOOKUP_BITSTRING;
    }
    else if (interpretation == "LookupBitmask")
    {
        return LOOKUP_BITMASK;
    }
    else
    {
        return NO_INTERPRETATION;
    }
}

MetadataTable::Alignment MetadataTable::GetAlignment() const
{
    string alignment = GetStringAttribute("Alignment");
    if (alignment == "Left")
    {
        return LEFT;
    }
    else if (alignment == "Right")
    {
        return RIGHT;
    }
    else if (alignment == "Center")
    {
        return CENTER;
    }
    else if (alignment == "Justify")
    {
        return JUSTIFY;
    }
    else
    {
        return NO_ALIGNMENT;
    }
}

bool MetadataTable::UseSeparator() const
{
    return GetBoolAttribute("UseSeparator");
}

string MetadataTable::GetEditMaskId() const
{
    return GetStringAttribute("EditMaskID");
}

string MetadataTable::GetLookupName() const
{
    return GetStringAttribute("LookupName");
}

int MetadataTable::GetMaxSelect() const
{
    return GetIntAttribute("MaxSelect");
}

MetadataTable::Units MetadataTable::GetUnits() const
{
    string units = GetStringAttribute("Units");
    if (units == "Feet")
    {
        return FEET;
    }
    else if (units == "Meters")
    {
        return METERS;
    }
    else if (units == "SqFt")
    {
        return SQUARE_FEET;
    }
    else if (units == "SqMeters")
    {
        return SQUARE_METERS;
    }
    else if (units == "Acres")
    {
        return ACRES;
    }
    else if (units == "Hectares")
    {
        return HECTARES;
    }
    else
    {
        return NO_UNITS;
    }
}

int MetadataTable::GetIndex() const
{
    return GetIntAttribute("Index");
}

int MetadataTable::GetMinimum() const
{
    return GetIntAttribute("Minimum");
}

int MetadataTable::GetMaximum() const
{
    return GetIntAttribute("Maximum");
}

int MetadataTable::GetDefault() const
{
    return GetIntAttribute("Default");
}

int MetadataTable::GetRequired() const
{
    return GetIntAttribute("Required");
}

string MetadataTable::GetSearchHelpId() const
{
    return GetStringAttribute("SearchHelpID");
}

bool MetadataTable::IsUnique() const
{
    return GetBoolAttribute("Unique");
}
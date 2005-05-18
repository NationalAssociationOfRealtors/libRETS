#include "librets/MetadataTable.h"

using namespace librets;
using std::string;

string MetadataTable::GetSystemName() const
{
    return "";
}

string MetadataTable::GetStandardName() const
{
    return "";
}

MetadataTable::DataType MetadataTable::GetDataType() const
{
    return BOOLEAN;
}

int MetadataTable::GetPrecision() const
{
    return 0;
}

bool MetadataTable::IsSearchable() const
{
    return false;
}

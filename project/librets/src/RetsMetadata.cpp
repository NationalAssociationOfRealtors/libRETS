#include "librets.h"

using namespace librets;
using std::string;
using std::vector;

RetsMetadata::RetsMetadata(MetadataElementPtr system)
{
}

MetadataSystemPtr RetsMetadata::GetSystem() const
{
    MetadataSystemPtr metadataElement;
    return metadataElement;
}

MetadataClassListPtr RetsMetadata::GetAllClasses() const
{
    MetadataClassListPtr allClasses;
    return allClasses;
}

MetadataClassPtr RetsMetadata::GetClass(string resourceName, string className)
    const
{
    MetadataClassPtr metadataClass;
    return metadataClass;
}

MetadataTableListPtr RetsMetadata::GetTablesForClass(
    MetadataClassPtr metadataClass) const
{
    MetadataTableListPtr tables;
    return tables;
}

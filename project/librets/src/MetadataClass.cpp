#include "librets/MetadataClass.h"

using namespace librets;
using std::string;

MetadataElement::MetadataType MetadataClass::GetType() const
{
    return CLASS;
}

string MetadataClass::GetClassName() const
{
    return GetStringAttribute("ClassName");
}

string MetadataClass::GetStandardName() const
{
    return GetStringAttribute("StandardName");
}

string MetadataClass::GetResourceName() const
{
    return "";
}

#include "librets/MetadataElement.h"

using namespace librets;
using std::string;
using std::vector;

MetadataElement::MetadataType MetadataElement::GetType() const
{
    return SYSTEM;
}

string MetadataElement::GetAttribute(string attributeName) const
{
    return "";
}

vector<MetadataElementPtr> MetadataElement::GetChildren(MetadataType type)
{
    vector<MetadataElementPtr> children;
    return children;
}

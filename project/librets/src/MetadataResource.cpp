#include "librets/MetadataResource.h"

using namespace librets;
using std::string;

#define CLASS MetadataResource

MetadataElement::MetadataType CLASS::GetType() const
{
    return RESOURCE;
}

string CLASS::GetResourceID() const
{
    return GetStringAttribute("ResourceID");
}

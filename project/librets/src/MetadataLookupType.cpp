#include "librets/MetadataLookupType.h"

using namespace librets;
using std::string;

#define CLASS MetadataLookupType

MetadataElement::MetadataType CLASS::GetType() const
{
    return LOOKUP_TYPE;
}

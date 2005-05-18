#include "librets/MetadataLookup.h"

using namespace librets;
using std::string;

#define CLASS MetadataLookup

MetadataElement::MetadataType CLASS::GetType() const
{
    return LOOKUP;
}

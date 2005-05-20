#include "librets/MetadataValidationLookup.h"

using namespace librets;
using std::string;

#define CLASS MetadataValidationLookup

MetadataElement::MetadataType CLASS::GetType() const
{
    return VALIDATION_LOOKUP;
}

#include "librets/MetadataValidationLookupType.h"

using namespace librets;
using std::string;

#define CLASS MetadataValidationLookupType

MetadataElement::MetadataType CLASS::GetType() const
{
    return VALIDATION_LOOKUP_TYPE;
}

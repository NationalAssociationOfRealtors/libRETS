#include "librets/MetadataValidationExternalType.h"

using namespace librets;
using std::string;

#define CLASS MetadataValidationExternalType

MetadataElement::MetadataType CLASS::GetType() const
{
    return VALIDATION_EXTERNAL_TYPE;
}

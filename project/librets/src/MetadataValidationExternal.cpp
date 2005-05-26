#include "librets/MetadataValidationExternal.h"

using namespace librets;
using std::string;

#define CLASS MetadataValidationExternal

MetadataElement::MetadataType CLASS::GetType() const
{
    return VALIDATION_EXTERNAL;
}

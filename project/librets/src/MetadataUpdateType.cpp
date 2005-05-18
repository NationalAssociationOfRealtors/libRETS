#include "librets/MetadataUpdateType.h"

using namespace librets;
using std::string;

#define CLASS MetadataUpdateType

MetadataElement::MetadataType CLASS::GetType() const
{
    return UPDATE_TYPE;
}

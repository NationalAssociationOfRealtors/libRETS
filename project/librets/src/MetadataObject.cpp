#include "librets/MetadataObject.h"

using namespace librets;
using std::string;

#define CLASS MetadataObject

MetadataElement::MetadataType CLASS::GetType() const
{
    return OBJECT;
}

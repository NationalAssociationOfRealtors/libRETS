#include "librets/MetadataEditMask.h"

using namespace librets;
using std::string;

#define CLASS MetadataEditMask

MetadataElement::MetadataType CLASS::GetType() const
{
    return EDIT_MASK;
}

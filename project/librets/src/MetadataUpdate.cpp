#include "librets/MetadataUpdate.h"

using namespace librets;
using std::string;

#define CLASS MetadataUpdate

MetadataElement::MetadataType CLASS::GetType() const
{
    return UPDATE;
}

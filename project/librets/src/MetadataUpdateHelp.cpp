#include "librets/MetadataUpdateHelp.h"

using namespace librets;
using std::string;

#define CLASS MetadataUpdateHelp

MetadataElement::MetadataType CLASS::GetType() const
{
    return UPDATE_HELP;
}

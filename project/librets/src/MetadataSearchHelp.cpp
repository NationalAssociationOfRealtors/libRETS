#include "librets/MetadataSearchHelp.h"

using namespace librets;
using std::string;

#define CLASS MetadataSearchHelp

MetadataElement::MetadataType CLASS::GetType() const
{
    return SEARCH_HELP;
}

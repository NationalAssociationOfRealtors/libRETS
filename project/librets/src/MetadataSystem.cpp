#include "librets/MetadataSystem.h"

using namespace librets;
using std::string;

MetadataSystem::~MetadataSystem()
{
}

MetadataElement::MetadataType MetadataSystem::GetType() const
{
    return SYSTEM;
}

string MetadataSystem::GetSystemID() const
{
    return GetStringAttribute("SystemID");
}

string MetadataSystem::GetSystemDescription() const
{
    return GetStringAttribute("SystemDescription");
}
 
string MetadataSystem::GetComments() const
{
    return GetStringAttribute("COMMENTS");
}

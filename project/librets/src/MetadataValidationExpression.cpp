#include "librets/MetadataValidationExpression.h"

using namespace librets;
using std::string;

#define CLASS MetadataValidationExpression

MetadataElement::MetadataType CLASS::GetType() const
{
    return VALIDATION_EXPRESSION;
}

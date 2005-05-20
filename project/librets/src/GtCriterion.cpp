#include <iostream>
#include "librets/GtCriterion.h"

using namespace librets;
using std::string;
using std::ostream;

GtCriterion::GtCriterion(string field, DmqlCriterionPtr value)
    : FieldCriterion(field, value)
{
}

ostream & GtCriterion::ToDmql(ostream & out) const
{
    out << "(" << mField << "=";
    return mValue->ToDmql(out) << "+)";
}

string GtCriterion::OperationName() const
{
    return "greater than";
}

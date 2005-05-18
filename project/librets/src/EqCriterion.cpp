#include <iostream>
#include "librets/EqCriterion.h"

using namespace librets;
using std::string;
using std::ostream;

EqCriterion::EqCriterion(string field, DmqlCriterionPtr value)
    : FieldCriterion(field, value)
{
}

ostream & EqCriterion::ToDmql(ostream & out) const
{
    out << "(" << mField << "=";
    return mValue->ToDmql(out) << ")";
}

string EqCriterion::OperationName() const
{
    return "equals";
}

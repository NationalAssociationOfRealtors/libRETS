#include <iostream>
#include "librets/LtCriterion.h"

using namespace librets;
using std::string;
using std::ostream;

LtCriterion::LtCriterion(string field, DmqlCriterionPtr value)
    : FieldCriterion(field, value)
{
}

ostream & LtCriterion::ToDmql(ostream & out) const
{
    out << "(" << mField << "=";
    return mValue->ToDmql(out) << "-)";
}

string LtCriterion::OperationName() const
{
    return "less than";
}

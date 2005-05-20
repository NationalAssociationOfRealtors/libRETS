#include <iostream>
#include "librets/NotCriterion.h"

using namespace librets;
using std::string;
using std::ostream;

NotCriterion::NotCriterion(DmqlCriterionPtr criterion)
    : mCriterion(criterion)
{
}

ostream & NotCriterion::ToDmql(ostream & out) const
{
    out << "~";
    return mCriterion->ToDmql(out);
}

ostream & NotCriterion::Print(ostream & out) const
{
    return out << "(Not <" << mCriterion << ">)";
}

bool NotCriterion::Equals(const RetsObject * object) const
{
    const NotCriterion * rhs =
        dynamic_cast<const NotCriterion *>(object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (*mCriterion == *rhs->mCriterion);
    return equals;
}

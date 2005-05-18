#include <iostream>
#include "librets/LiteralCriterion.h"

using namespace librets;
using std::string;

LiteralCriterion::LiteralCriterion()
{
}

LiteralCriterion::LiteralCriterion(string aString)
    : mString(aString)
{
}

std::ostream & LiteralCriterion::ToDmql(std::ostream & outputStream) const
{
    return outputStream << mString;
}

std::ostream & LiteralCriterion::Print(std::ostream & outputStream) const
{
    return outputStream << "Literal Criterion [" << mString << "]";
}

bool LiteralCriterion::Equals(const RetsObject * object) const
{
    const LiteralCriterion * rhs =
        dynamic_cast<const LiteralCriterion *>(object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mString == rhs->mString);
    return equals;
}

#include <iostream>
#include "librets/FieldCriterion.h"

using namespace librets;
using std::string;
using std::ostream;

FieldCriterion::FieldCriterion(string field, DmqlCriterionPtr value)
    : mField(field), mValue(value)
{
}

ostream & FieldCriterion::Print(ostream & outputStream) const
{
    return outputStream << "(<" << mField << "> " << OperationName() << " <"
                        << mValue << ">)";
}

bool FieldCriterion::Equals(const RetsObject * object) const
{
    const FieldCriterion * rhs =
        dynamic_cast<const FieldCriterion *>(object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mField == rhs->mField);
    equals &= (OperationName() == rhs->OperationName());
    equals &= (*mValue == *rhs->mValue);
    return equals;
}

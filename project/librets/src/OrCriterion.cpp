#include <iostream>
#include "librets/OrCriterion.h"

using namespace librets;
using std::string;
using std::ostream;

OrCriterion::OrCriterion(DmqlCriterionPtr first, DmqlCriterionPtr second)
    : JunctionCriterion()
{
    // Note that virtual methods are disabled during constructors, so
    // it is important to call add() from within scope of OrCriterion
    // to ensure the proper methods get called.
    add(first);
    add(second);
}

string OrCriterion::Operator() const
{
    return "|";
}

string OrCriterion::OperationName() const
{
    return "locical or";
}

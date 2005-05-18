#include <iostream>
#include "librets/AndCriterion.h"

using namespace librets;
using std::string;
using std::ostream;

AndCriterion::AndCriterion(DmqlCriterionPtr first, DmqlCriterionPtr second)
    : JunctionCriterion()
{
    // Note that virtual methods are disabled during constructors, so
    // it is important to call add() from within scope of AndCriterion
    // to ensure the proper methods get called.
    add(first);
    add(second);
}

string AndCriterion::Operator() const
{
    return ",";
}

string AndCriterion::OperationName() const
{
    return "locical and";
}

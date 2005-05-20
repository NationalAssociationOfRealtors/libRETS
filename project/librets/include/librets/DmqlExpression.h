#ifndef LIBRETS_DMQL_EXPRESSION_H
#define LIBRETS_DMQL_EXPRESSION_H

#include "librets/sql_forward.h"

namespace librets {
namespace DmqlExpression {

DmqlCriterionPtr literal(std::string aString);

DmqlCriterionPtr literal(int number);

DmqlCriterionPtr eq(std::string field, DmqlCriterionPtr value);

DmqlCriterionPtr gt(std::string field, DmqlCriterionPtr value);

DmqlCriterionPtr lt(std::string field, DmqlCriterionPtr value);

DmqlCriterionPtr logicOr(DmqlCriterionPtr first, DmqlCriterionPtr second);

DmqlCriterionPtr logicAnd(DmqlCriterionPtr first, DmqlCriterionPtr second);

DmqlCriterionPtr logicNot(DmqlCriterionPtr criterion);

}; };

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

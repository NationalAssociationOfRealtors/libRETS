#ifndef LIBRETS_AND_CRITERION_H
#define LIBRETS_AND_CRITERION_H

#include <string>
#include "librets/sql_forward.h"
#include "librets/JunctionCriterion.h"

namespace librets {

class AndCriterion : public JunctionCriterion
{
  public:
    AndCriterion(DmqlCriterionPtr first, DmqlCriterionPtr second);

  protected:
    virtual std::string Operator() const;
    virtual std::string OperationName() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

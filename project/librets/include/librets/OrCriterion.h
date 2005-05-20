#ifndef LIBRETS_OR_CRITERION_H
#define LIBRETS_OR_CRITERION_H

#include <string>
#include "librets/sql_forward.h"
#include "librets/JunctionCriterion.h"

namespace librets {

class OrCriterion : public JunctionCriterion
{
  public:
    OrCriterion(DmqlCriterionPtr first, DmqlCriterionPtr second);

  protected:
    virtual std::string Operator() const;

    virtual std::string OperationName() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

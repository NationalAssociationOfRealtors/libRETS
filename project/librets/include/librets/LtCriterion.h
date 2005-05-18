#ifndef LIBRETS_LT_CRITERION_H
#define LIBRETS_LT_CRITERION_H

#include <string>
#include "librets/sql_forward.h"
#include "librets/FieldCriterion.h"

namespace librets {

class LtCriterion : public FieldCriterion
{
  public:
    LtCriterion(std::string field, DmqlCriterionPtr value);

    virtual std::ostream & ToDmql(std::ostream & outputStream) const;

  protected:
    virtual std::string OperationName() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

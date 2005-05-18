#ifndef LIBRETS_NOT_CRITERION_H
#define LIBRETS_NOT_CRITERION_H

#include <string>
#include "librets/sql_forward.h"
#include "librets/DmqlCriterion.h"

namespace librets {

class NotCriterion : public DmqlCriterion
{
  public:
    NotCriterion(DmqlCriterionPtr criterion);

    virtual std::ostream & ToDmql(std::ostream & out) const;

    virtual std::ostream & Print(std::ostream & out) const;

    virtual bool Equals(const RetsObject * object) const;

  private:
    DmqlCriterionPtr mCriterion;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

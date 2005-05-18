#ifndef LIBRETS_FIELD_CRITERION_H
#define LIBRETS_FIELD_CRITERION_H

#include <string>
#include "librets/sql_forward.h"
#include "librets/DmqlCriterion.h"

namespace librets {

class FieldCriterion : public DmqlCriterion
{
  public:
    FieldCriterion(std::string field, DmqlCriterionPtr value);

    virtual std::ostream & Print(std::ostream & outputStream) const;

    bool Equals(const RetsObject * object) const;

  protected:
    virtual std::string OperationName() const = 0;

    std::string mField;
    DmqlCriterionPtr mValue;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

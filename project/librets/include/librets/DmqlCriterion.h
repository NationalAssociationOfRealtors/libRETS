#ifndef LIBRETS_DMQL_CRITERION_H
#define LIBRETS_DMQL_CRITERION_H

#include "librets/RetsObject.h"

namespace librets {

class DmqlCriterion : public virtual RetsObject
{
  public:
    virtual ~DmqlCriterion();

    virtual std::ostream & ToDmql(std::ostream & outputStream) const = 0;

    std::string ToDmqlString();
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

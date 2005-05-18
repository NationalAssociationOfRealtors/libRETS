#ifndef LIBRETS_LITERAL_CRITERION_H
#define LIBRETS_LITERAL_CRITERION_H

#include <string>
#include "librets/DmqlCriterion.h"

namespace librets {

class LiteralCriterion : public DmqlCriterion
{
  public:
    LiteralCriterion();
    LiteralCriterion(std::string aString);

    virtual std::ostream & ToDmql(std::ostream & outputStream) const;

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * object) const;

  private:
    std::string mString;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

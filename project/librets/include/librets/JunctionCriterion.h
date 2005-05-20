#ifndef LIBRETS_JUNCTION_CRITERION_H
#define LIBRETS_JUNCTION_CRITERION_H

#include <string>
#include <vector>
#include "librets/sql_forward.h"
#include "librets/DmqlCriterion.h"

namespace librets {

class JunctionCriterion : public DmqlCriterion
{
  public:
    JunctionCriterion();
    JunctionCriterion(DmqlCriterionPtr first, DmqlCriterionPtr second);

    void add(DmqlCriterionPtr criterion);

    virtual std::ostream & ToDmql(std::ostream & outputStream) const;

    virtual std::ostream & Print(std::ostream & outputStream) const;

    bool Equals(const RetsObject * object) const;

  protected:
    typedef std::vector<DmqlCriterionPtr> CriterionList;

    void addAll(JunctionCriterionPtr junction);
    void addAll(const CriterionList & criteria);

    virtual std::string OperationName() const = 0;
    virtual std::string Operator() const = 0;

    CriterionList mCriteria;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

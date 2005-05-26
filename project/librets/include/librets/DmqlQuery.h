#ifndef LIBRETS_DMQL_QUERY_H
#define LIBRETS_DMQL_QUERY_H

#include "librets/std_forward.h"
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class DmqlQuery : public RetsObject
{
  public:
    DmqlQuery();

    std::string GetResource() const;

    void SetResource(std::string resource);

    std::string GetClass() const;

    void SetClass(std::string aClass);

    StringVectorPtr GetFields() const;

    void AddField(std::string column);

    DmqlCriterionPtr GetCriterion() const;

    void SetCriterion(DmqlCriterionPtr criterion);

    virtual std::ostream & Print(std::ostream & outputStream) const;

  private:
    std::string mResource;
    std::string mClass;
    StringVectorPtr mFields;
    DmqlCriterionPtr mCriterion;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

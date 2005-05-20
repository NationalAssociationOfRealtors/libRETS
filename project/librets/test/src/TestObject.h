#ifndef LIBRETS_TEST_OBJECT_H
#define LIBRETS_TEST_OBJECT_H

#include <boost/shared_ptr.hpp>
#include "librets/RetsObject.h"

namespace librets {

class TestObject : public RetsObject
{
  public:
    TestObject(int number) : mNumber(number) { };
    virtual ~TestObject();
    virtual std::ostream & Print(std::ostream & s) const;
    virtual bool Equals(const RetsObject * rhs) const;

  private:
    int mNumber;
};

typedef boost::shared_ptr<TestObject> TestObjectPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

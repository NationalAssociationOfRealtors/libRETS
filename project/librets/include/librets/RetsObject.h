#ifndef LIBRETS_RETS_OBJECT_H
#define LIBRETS_RETS_OBJECT_H

#include <iosfwd>
#include <string>
#include <boost/shared_ptr.hpp>

namespace librets {

class RetsObject
{
  public:
    virtual ~RetsObject();

    virtual std::ostream & Print(std::ostream & outputStream) const;

    std::string ToString() const;

    virtual bool Equals(const RetsObject * object) const;
};

/** Smart pointer to RetsObject. */
typedef boost::shared_ptr<RetsObject> RetsObjectPtr;

std::ostream & operator<<(std::ostream & s, const RetsObject & retsObject);

std::ostream & operator<<(std::ostream & s, RetsObject * retsObject);

bool operator==(const RetsObject & lhs, const RetsObject & rhs);

bool operator!=(const RetsObject & lhs, const RetsObject & rhs);

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

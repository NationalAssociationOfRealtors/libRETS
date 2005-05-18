#ifndef LIBRETS_RETS_XML_ATTRIBUTE_H
#define LIBRETS_RETS_XML_ATTRIBUTE_H

#include <string>
#include "librets/RetsObject.h"

namespace librets {

class RetsXmlAttribute : public RetsObject
{
  public:
    RetsXmlAttribute(std::string name, std::string value);

    std::string GetName() const;

    std::string GetValue() const;

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * rhs) const;

  private:
    std::string mName;
    std::string mValue;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

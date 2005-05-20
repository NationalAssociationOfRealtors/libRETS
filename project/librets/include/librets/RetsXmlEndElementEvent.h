#ifndef LIBRETS_RETS_XML_END_ELEMENT_EVENT_H
#define LIBRETS_RETS_XML_END_ELEMENT_EVENT_H

#include "librets/RetsXmlEvent.h"

namespace librets {

class RetsXmlEndElementEvent : public RetsXmlEvent
{
  public:
    virtual ~RetsXmlEndElementEvent();

    virtual Type GetType() const;

    void SetName(std::string name);

    std::string GetName() const;

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * rhs) const;

  private:
    std::string mName;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

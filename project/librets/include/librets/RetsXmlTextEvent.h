#ifndef LIBRETS_RETS_XML_TEXT_EVENT_H
#define LIBRETS_RETS_XML_TEXT_EVENT_H

#include <sstream>
#include "librets/RetsXmlEvent.h"

namespace librets {

class RetsXmlTextEvent : public RetsXmlEvent
{
  public:
    virtual ~RetsXmlTextEvent();

    virtual Type GetType() const;

    void AppendText(std::string text);

    std::string GetText() const;

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * rhs) const;

  private:
    std::ostringstream mText;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

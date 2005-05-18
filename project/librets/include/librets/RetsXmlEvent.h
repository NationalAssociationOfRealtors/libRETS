#ifndef LIBRETS_RETS_XML_EVENT_H
#define LIBRETS_RETS_XML_EVENT_H

#include <string>
#include "librets/RetsObject.h"

namespace librets {

class RetsXmlEvent : public RetsObject
{
  public:
    enum Type {
        START_ELEMENT,
        END_ELEMENT,
        TEXT
    };
    
    virtual ~RetsXmlEvent();

    virtual Type GetType() const = 0;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

#ifndef LIBRETS_METADATA_ELEMENT_COLLECTOR_H
#define LIBRETS_METADATA_ELEMENT_COLLECTOR_H

#include "librets/metadata_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class MetadataElementCollector : public virtual RetsObject
{
  public:
    virtual ~MetadataElementCollector();

    virtual void AddElement(MetadataElementPtr element) = 0;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

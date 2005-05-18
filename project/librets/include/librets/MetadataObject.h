#ifndef LIBRETS_METADATA_OBJECT_H
#define LIBRETS_METADATA_OBJECT_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Object metadata.
 */
class MetadataObject : public MetadataElement
{
  public:
    /**
     * Always returns OBJECT.
     *
     * @return OBJECT
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

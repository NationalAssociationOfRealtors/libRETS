#ifndef LIBRETS_METADATA_LOOKUP_TYPE_H
#define LIBRETS_METADATA_LOOKUP_TYPE_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Lookup type metadata.
 */
class MetadataLookupType : public MetadataElement
{
  public:
    /**
     * Always returns LOOKUP_TYPE.
     *
     * @return LOOKUP_TYPE
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

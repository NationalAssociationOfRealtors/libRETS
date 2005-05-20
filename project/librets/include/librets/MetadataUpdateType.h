#ifndef LIBRETS_METADATA_UPDATE_TYPE_H
#define LIBRETS_METADATA_UPDATE_TYPE_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Update type metadata.
 */
class MetadataUpdateType : public MetadataElement
{
  public:
    /**
     * Always returns UPDATE_TYPE.
     *
     * @return UPDATE_TYPE
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

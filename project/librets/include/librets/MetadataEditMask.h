#ifndef LIBRETS_METADATA_EDIT_MASK_H
#define LIBRETS_METADATA_EDIT_MASK_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Edit mask metadata.
 */
class MetadataEditMask : public MetadataElement
{
  public:
    /**
     * Always returns EDIT_MASK.
     *
     * @return EDIT_MASK
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

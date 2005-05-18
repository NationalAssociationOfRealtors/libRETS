#ifndef LIBRETS_METADATA_VALIDATION_EXTERNAL_TYPE_H
#define LIBRETS_METADATA_VALIDATION_EXTERNAL_TYPE_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Validation external type metadata.
 */
class MetadataValidationExternalType : public MetadataElement
{
  public:
    /**
     * Awlays returns VALIDATION_EXTERNAL_TYPE.
     *
     * @return VALIDATION_EXTERNAL_TYPE
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

#ifndef LIBRETS_METADATA_VALIDATION_LOOKUP_TYPE_H
#define LIBRETS_METADATA_VALIDATION_LOOKUP_TYPE_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Validation lookup type metadata.
 */
class MetadataValidationLookupType : public MetadataElement
{
  public:
    /**
     * Always returns VALIDATION_LOOKUP_TYPE.
     *
     * @return VALIDATION_LOOLKUP_TYPE
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

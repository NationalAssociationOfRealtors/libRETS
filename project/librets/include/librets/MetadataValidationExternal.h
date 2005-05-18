#ifndef LIBRETS_METADATA_VALIDATION_EXTERNAL_H
#define LIBRETS_METADATA_VALIDASTION_EXTERNAL_H

#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataValidationExternal objects. */
typedef std::vector<MetadataValidationExternalTypePtr>
    MetadataValidationExternalTypeList;
/** A smart pointer to MetadataValidationExternalTypeList. */
typedef boost::shared_ptr<MetadataValidationExternalTypeList>
    MetadataValidationExternalTypeListPtr;

/**
 * Validation external metadata.
 */
class MetadataValidationExternal : public MetadataElement
{
  public:
    /**
     * Always returns VALIDATION_EXTERNAL.
     *
     * @return VALIDATION_EXTERNAL
     */
    virtual MetadataType GetType() const;

    /**
     * Returns all validation external types.
     *
     * @return All validation external types
     */
    MetadataValidationExternalTypeListPtr GetValidationExternalTypes() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

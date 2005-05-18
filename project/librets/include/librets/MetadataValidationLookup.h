#ifndef LIBRETS_METADATA_VALIDATION_LOOKUP_H
#define LIBRETS_METADATA_VALIDATION_LOOKUP_H

#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataValidationLookupType objects. */
typedef std::vector<MetadataValidationLookupTypePtr>
    MetadataValidationLookupTypeList;
/** A smart pointer to MetadataValidationLookupTypeList. */
typedef boost::shared_ptr<MetadataValidationLookupTypeList>
    MetadataValidationLookupTypeListPtr;

/**
 * Validation lookup metadata.
 */
class MetadataValidationLookup : public MetadataElement
{
  public:
    /**
     * Always returns VALIDATION_LOOKUP.
     *
     * @return VALIDATION_LOOKUP
     */
    virtual MetadataType GetType() const;

    /**
     * Returns all validation lookup types.
     *
     * @return All validation lookup types
     */
    MetadataValidationLookupTypeListPtr GetValidationLookupTypes() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

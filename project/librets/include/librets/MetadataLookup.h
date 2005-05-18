#ifndef LIBRETS_METADATA_LOOKUP_H
#define LIBRETS_METADATA_LOOKUP_H

#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataLookup objects. */
typedef std::vector<MetadataLookupTypePtr> MetadataLookupTypeList;
/** A smart pointer to MetadataLookupType. */
typedef boost::shared_ptr<MetadataLookupTypeList> MetadataLookupTypeListPtr;

/**
 * Lookup metadata.
 */
class MetadataLookup : public MetadataElement
{
  public:
    /**
     * Always returns LOOKUP.
     *
     * @return LOOKUP
     */
    virtual MetadataType GetType() const;

    /**
     * Returns all lookup types.
     *
     * @return All lookup types
     */
    MetadataLookupTypeListPtr GetLookupTypes() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

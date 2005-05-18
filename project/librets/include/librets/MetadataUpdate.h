#ifndef LIBRETS_METADATA_UPDATE_H
#define LIBRETS_METADATA_UPDATE_H

#include "librets/MetadataUpdate.h"

namespace librets {

/** A vector of MetadataUpdateType objects. */
typedef std::vector<MetadataUpdateTypePtr> MetadataUpdateTypeList;
/** A smart pointer to MetadataUpdateTypeList. */
typedef boost::shared_ptr<MetadataUpdateTypeList> MetadataUpdateTypeListPtr;

/**
 * Update metadata.
 */
class MetadataUpdate : public MetadataElement
{
  public:
    /**
     * Alwyas returns UPDATE.
     *
     * @return UPDATE
     */
    virtual MetadataType GetType() const;

    /**
     * Returns all update types.
     *
     * @return All update types
     */
    MetadataUpdateTypeListPtr GetUpdateTypes() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

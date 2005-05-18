#ifndef LIBRETS_METADATA_SYSTEM_H
#define LIBRETS_METADATA_SYSTEM_H

#include <string>
#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataResource objecst. */
typedef std::vector<MetadataResourcePtr> MetadataResourceList;
/** A smart pointer to MetadataResourceList. */
typedef boost::shared_ptr<MetadataResourceList> MetadataResourceListPtr;

/** A vector of MetadataForeignKey objects. */
typedef std::vector<MetadataForeignKeyPtr> MetadataForeignKeyList;
/** A smart pointer to MetadataForeignKeyList. */
typedef boost::shared_ptr<MetadataForeignKeyList> MetadataForeignKeyListPtr;

/**
 * System metadata.
 */
class MetadataSystem : public MetadataElement
{
  public:
    /**
     * Returns SYSTEM.
     */
    virtual MetadataType GetType() const;

    /**
     * Returns the system ID.
     *
     * @return The system ID
     */
    std::string GetSystemID() const;

    /**
     * Returns the system description.
     *
     * @return The system description
     */
    std::string GetSystemDescription() const;

    /**
     * Returns the comments.
     *
     * @return The comments
     */
    std::string GetComments() const;

    /**
     * Returns all the child resources.
     *
     * @return All the child resources
     */
    MetadataResourceListPtr GetResources() const;

    /**
     * Returns all the child foreign keys.
     *
     * @return All the child foreign keys
     */
    MetadataForeignKeyListPtr GetForeignKeys() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

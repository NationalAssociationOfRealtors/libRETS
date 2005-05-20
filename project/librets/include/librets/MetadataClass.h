#ifndef LIBRETS_METADATA_CLASS_H
#define LIBRETS_METADATA_CLASS_H

#include <string>

#include "librets/MetadataElement.h"

namespace librets {

/** Vector of MetadataTable objects. */
typedef std::vector<MetadataTablePtr> MetadataTableList;
/** A smart pointer to MetadataTableList. */
typedef boost::shared_ptr<MetadataTableList> MetadataTableListPtr;

/** A vector of MetadataUpdate objects. */
typedef std::vector<MetadataUpdatePtr> MetadataUpdateList;
/** A smart pointer to MetadataUpdateList. */
typedef boost::shared_ptr<MetadataUpdateList> MetadataUpdateListPtr;

/**
 * Class metadata.
 */
class MetadataClass : public MetadataElement
{
  public:
    /**
     * Always returns CLASS;
     *
     * @return CLASS
     */
    virtual MetadataType GetType() const;

    /**
     * Returns the name of the class metadata.
     *
     * @return Name of the class metadata.
     */
    std::string GetClassName() const;

    std::string GetStandardName() const;

    /**
     * Returns the resource name of the class metadata.
     *
     * @return Name of the resource metadata.
     */
    std::string GetResourceName() const;

    /**
     * Returns all tables.
     *
     * @return All tables
     */
    MetadataTableListPtr GetTables() const;

    /**
     * Returns all updates.
     *
     * @return All updtes
     */
    MetadataUpdateListPtr GetUpdates() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

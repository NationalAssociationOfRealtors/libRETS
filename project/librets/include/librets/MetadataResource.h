#ifndef LIBRETS_METADATA_RESOURCE_H
#define LIBRETS_METADATA_RESOURCE_H

#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataClass objects. */
typedef std::vector<MetadataClassPtr> MetadataClassList;
/** A smart pointer to MetadataClassList. */
typedef boost::shared_ptr<MetadataClassList> MetadataClassListPtr;

/** A vector of MetadataObject objects.*/
typedef std::vector<MetadataObjectPtr> MetadataObjectList;
/** A smart pointer to MetadataObjectList. */
typedef boost::shared_ptr<MetadataObjectList> MetadataObjectListPtr;

/** A vector of MetadataSearchHelp objects. */
typedef std::vector<MetadataSearchHelpPtr> MetadataSearchHelpList;
/** A smart pointer to MetadataSearchHelpList. */
typedef boost::shared_ptr<MetadataSearchHelpList> MetadataSearchHelpListPtr;

/** A vector of MetadataEditMask objects. */
typedef std::vector<MetadataEditMaskPtr> MetadataEditMaskList;
/** A smart pointer to MetadataEditMaskList. */
typedef boost::shared_ptr<MetadataEditMaskList> MetadataEditMaskListPtr;

/** A vector of MetadataUpdateHelp objects. */
typedef std::vector<MetadataUpdateHelpPtr> MetadataUpdateHelpList;
/** A smart pointer to MetadataUPdateHelpList. */
typedef boost::shared_ptr<MetadataUpdateHelpList> MetadataUpdateHelpListPtr;

/** A vector of MetadataLookup objects. */
typedef std::vector<MetadataLookupPtr> MetadataLookupList;
/** A smart pointer to MetadataLookupList. */
typedef boost::shared_ptr<MetadataLookupList> MetadataLookupListPtr;

/** A vector of MetadataValidationLookup objects. */
typedef std::vector<MetadataValidationLookupPtr> MetadataValidationLookupList;
/** A smart pointer to MetadataValidationLookupList. */
typedef boost::shared_ptr<MetadataValidationLookupList>
    MetadataValidationLookupListPtr;

/** A vector of MetadataValidationExternal objects. */
typedef std::vector<MetadataValidationExternalPtr>
    MetadataValidationExternalList;
/** A smart pointer to MetadataValidationExternalList. */
typedef boost::shared_ptr<MetadataValidationExternalList>
    MetadataValidationExternalListPtr;

/** A vector of MetadataValidationExpression objects. */
typedef std::vector<MetadataValidationExpressionPtr>
    MetadataValidationExpressionList;
/** A smart pointer to MetadataValidationExpressionList. */
typedef boost::shared_ptr<MetadataValidationExpressionList>
    MetadataValidationExpressionListPtr;

/**
 * Resource metadata.
 */
class MetadataResource : public MetadataElement
{
  public:
    /**
     * Always returns RESOURCE.
     *
     * @return RESOURCE
     */
    virtual MetadataType GetType() const;

    /**
     * Returns all classes.
     *
     * @return All classes
     */
    MetadataClassListPtr GetClasses() const;

    /**
     * Returns all objects.
     *
     * @return All objects
     */
    MetadataObjectListPtr GetObjects() const;

    /**
     * Return all search helps.
     *
     * @return All search helps
     */
    MetadataSearchHelpListPtr GetSearchHelps() const;

    /**
     * Return all edit masks.
     *
     * @return All edit masks
     */
    MetadataEditMaskListPtr GetEditMasks() const;

    /**
     * Return all update helps.
     *
     * @return All update helps
     */
    MetadataUpdateHelpListPtr GetUpdateHelps() const;

    /**
     * Return all lookups.
     *
     * @return All lookups
     */
    MetadataLookupListPtr GetLookups() const;

    /**
     * Return all validation lookups.
     *
     * @return All validation lookups
     */
    MetadataValidationLookupListPtr GetValidationLookups() const;

    /**
     * Return all validation externals.
     *
     * @return All validation externals
     */
    MetadataValidationExternalListPtr GetValidationExternals() const;

    /**
     * Return all validation expressions.
     *
     * @return All validation expressions
     */
    MetadataValidationExpressionListPtr GetValidationExpressions() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

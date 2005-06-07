#ifndef LIBRETS_RETS_METADATA_H
#define LIBRETS_RETS_METADATA_H

#include <string>
#include <vector>
#include "librets/metadata_forward.h"

namespace librets {

/** A vector of MetadataResource objects. */
typedef std::vector<MetadataResourcePtr> MetadataResourceList;
/** A smart pointer to MetadataResourceList. */
typedef boost::shared_ptr<MetadataResourceList> MetadataResourceListPtr;

/** A vector of MetadataClass objects. */
typedef std::vector<MetadataClassPtr> MetadataClassList;
/** A smart pointer to MetadataClassList. */
typedef boost::shared_ptr<MetadataClassList> MetadataClassListPtr;

/** A vector of MetadataTables objects */
typedef std::vector<MetadataTablePtr> MetadataTableList;
/** A smart pointer to MetadataTableList. */
typedef boost::shared_ptr<MetadataTableList> MetadataTableListPtr;

/**
 * Contains all the metadata.
 */
class RetsMetadata
{
  public:
    /**
     * Creates a metadata from a metadata by-level collector.
     */
    RetsMetadata(MetadataByLevelCollectorPtr collector);

    /**
     * Returns the system metdata element.
     *
     * @return The system metadata element
     */
    MetadataSystemPtr GetSystem() const;

    /**
     * Returns all the class metadata elements.
     *
     * @return All the class metadata elements
     */
    MetadataClassListPtr GetAllClasses() const;

    /**
     * Returns all the resource metadata elements.
     *
     * @return All the resource metadata elemens
     */
    MetadataResourceListPtr GetAllResources() const;

    /**
     * Returns the metadata resource from its resource names.
     *
     * @param resourceName A resource name
     * @return A metadata class
     */
    MetadataResourcePtr GetResource(std::string resourceName) const;

    /**
     * Returns all metadata class elements for a specified metadata class.
     *
     * @param resourceName A resource name
     * @return All metadata class elements for that resource
     */
    MetadataClassListPtr GetClassesForResource(std::string resourceName) const;
    
    /**
     * Returns the metadata class from its resource and class names.
     *
     * @param resourceName A resource name
     * @param className A class name
     * @return A metadata class
     */
    MetadataClassPtr GetClass(std::string resourceName, std::string className)
        const;

    /**
     * Returns the metadata class from its resource and class names.
     *
     * @param resourceName A resource name
     * @param className A class name
     * @param className A table name
     * @return A metadata class
     */
    MetadataTablePtr GetTable(std::string resourceName, std::string className,
                              std::string tableName) const;

    /**
     * Returns all metadata table elements for a specified
     * metadata class.
     *
     * @param metadataClass A metadata class element
     * @return All metadata table elements
     */
    MetadataTableListPtr
        GetTablesForClass(MetadataClassPtr metadataClass) const;
    
    MetadataTableListPtr
        GetTablesForClass(std::string resourceName,
                          std::string className) const;

  private:
    void InitSystem();
    void InitAllClasses();
    void InitAllResources();

    MetadataByLevelCollectorPtr mCollector;
    MetadataSystemPtr mSystem;
    MetadataClassListPtr mAllClasses;
    MetadataResourceListPtr mAllResources;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

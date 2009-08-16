/*
 * Copyright (C) 2005-2009 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */

#ifndef LIBRETS_RETS_METADATA_H
#define LIBRETS_RETS_METADATA_H
/** 
 * @file RetsMetadata.h
 * Contains the RetsMetadata class declaration.
 */

#include <string>
#include <vector>
#include <set>
#include "librets/BinaryData.h"
#include "librets/EncodingType.h"
#include "librets/ExceptionErrorHandler.h"
#include "librets/metadata_forward.h"
#include "librets/MetadataElement.h"

namespace librets {

/** A vector of MetadataResource objects. */
typedef std::vector<MetadataResource *> MetadataResourceList;
/** A smart pointer to MetadataResourceList. */
typedef boost::shared_ptr<MetadataResourceList> MetadataResourceListPtr;

/** A vector of MetadataClass objects. */
typedef std::vector<MetadataClass *> MetadataClassList;
/** A smart pointer to MetadataClassList. */
typedef boost::shared_ptr<MetadataClassList> MetadataClassListPtr;

/** A vector of MetadataTables objects */
typedef std::vector<MetadataTable *> MetadataTableList;
/** A smart pointer to MetadataTableList. */
typedef boost::shared_ptr<MetadataTableList> MetadataTableListPtr;

/** A vector of MetadataLookup objects */
typedef std::vector<MetadataLookup *> MetadataLookupList;

/** A vector of MetadataLookupType objects */
typedef std::vector<MetadataLookupType *> MetadataLookupTypeList;

/** A vector of MetadataObject objects. */
typedef std::vector<MetadataObject *> MetadataObjectList;

/** A vector of MetadataForeignKeys objects */
typedef std::vector<MetadataForeignKey *> MetadataForeignKeyList;
/** A smart pointer to MetadataForeignKeyList. */
typedef boost::shared_ptr<MetadataForeignKeyList> MetadataForeignKeyListPtr;

/**
 * RetsMetadata contains the API that is the main interface to
 * all of the metadata.
 */
class RetsMetadata
{
  public:
    /// @cond MAINTAINER
    /**
     * Creates metadata from a metadata by-level collector.
     */
    RetsMetadata(MetadataFinderPtr finder);
    /// @endcond
    
    /**
     * Create and return a Metadata object that parses the data
     * from a given input stream.
     *
     * @param inputStream A stream from which the data will be injected
     * into the parser.
     * @param encoding The data encoding type: 
     * one of RETS_XML_DEFAULT_ENCODING (default),
     * RETS_XML_ISO_ENCODING or RETS_XML_UTF8_ENCODING
     * @param handler The Exception Error Handler
     * @return The default metadata object.
     */
    static RetsMetadata * CreateAndParse(
                        istreamPtr inputStream, 
                        EncodingType encoding = RETS_XML_DEFAULT_ENCODING,
                        ExceptionErrorHandler * handler = ExceptionErrorHandler::GetInstance());
    /**
     * Create and return a Metadata object that parses the data
     * from a BinaryData object.
     *
     * @param binaryData A BinaryData object from which the data will be injected
     * into the parser.
     * @param encoding The data encoding type: 
     * one of RETS_XML_DEFAULT_ENCODING (default),
     * RETS_XML_ISO_ENCODING or RETS_XML_UTF8_ENCODING
     * @param handler The Exception Error Handler
     * @return The default metadata object.
     */
    static RetsMetadata * CreateAndParse(
                        BinaryData binaryData,
                        EncodingType encoding = RETS_XML_DEFAULT_ENCODING,
                        ExceptionErrorHandler * handler = ExceptionErrorHandler::GetInstance());
    

#ifdef DOXYGEN
    /**
     * Create and return a Metadata object that parses the data
     * from a java byte array. This API is for Java only.
     *
     * @param buffer A Java byte array containing the raw RETS metadta.
     * @return A RetsMetadata object with data parsed from the array.
     */
    public static RetsMetadata CreateMetadataFromArray(byte [] buffer);
#endif

    /**
     * Returns the system metdata element.
     *
     * @return The system metadata element
     */
    MetadataSystem * GetSystem() const;

    /**
     * Retusn the foreign key metadata elements.
     *
     * @return All the foreign key metadata elements
     */
    MetadataForeignKeyList GetAllForeignKeys() const;
    
    /**
     * Returns all the resource metadata elements.
     *
     * @return All the resource metadata elements
     */
    MetadataResourceList GetAllResources() const;

    /**
     * Returns the metadata resource from its resource name.
     *
     * @param resourceName A resource name
     * @return A metadata class
     */
    MetadataResource * GetResource(std::string resourceName) const;

    /**
     * Returns all metadata class elements for a specified resource.
     *
     * @param resourceName A resource name
     * @return All metadata class elements for that resource
     */
    MetadataClassList GetAllClasses(std::string resourceName) const;
    
    /**
     * Returns the metadata class from its resource and class names.
     *
     * @param resourceName A resource name
     * @param className A class name
     * @return A metadata class
     */
    MetadataClass * GetClass(std::string resourceName, std::string className)
        const;

    /**
     * Returns all metadata table elements for a specified
     * metadata class.
     *
     * @param metadataClass A metadata class element
     * @return All metadata table elements
     */
    MetadataTableList GetAllTables(MetadataClass * metadataClass) const;
    
    /**
     * Returns all metadata table elements with a resource and class name.
     *
     * @param resourceName A resource name
     * @param className A class name
     * @return A list of matching metadata tables
     */
    MetadataTableList GetAllTables(std::string resourceName,
                                   std::string className) const;

    /**
     * Returns the metadata class from its resource and class names.
     *
     * @param resourceName A resource name
     * @param className A class name
     * @param tableName A table name
     * @return A metadata class
     */
    MetadataTable * GetTable(std::string resourceName, std::string className,
                             std::string tableName) const;
    
    /**
     * Returns all metadata lookup elements for a specified resource.
     *
     * @param resourceName A resource name
     * @return All lookups for that resource
     */
    MetadataLookupList GetAllLookups(std::string resourceName) const;

    /**
     * Returns a metadata lookup from a resource and lookup name.
     *
     * @param resourceName A resource name
     * @param lookupName A lookup name
     * @return A metadata lookup
     */
    MetadataLookup * GetLookup(std::string resourceName, std::string lookupName)
        const;
    
    /**
     * Returns all metadata lookup type element from its resource and lookup
     * names.
     *
     * @param resourceName A resource name
     * @param lookupName A lookup name
     * @return All lookup types
     */
    MetadataLookupTypeList GetAllLookupTypes(std::string resourceName,
                                             std::string lookupName) const;
    
    /**
     * Returns all metadata lookup type element for a lookup.
     *
     * @param metadataLookup A metadata lookup element
     * @return All lookup types
     */
    MetadataLookupTypeList GetAllLookupTypes(MetadataLookup * metadataLookup)
        const;
    
    /**
     * Returns a metadata lookup from a resource name, lookup name, and 
     * lookup value.
     *
     * @param resourceName A resource name
     * @param lookupName A lookup  name
     * @param lookupValue A lookup value
     * @return A metadata lookup type
     */
    MetadataLookupType * GetLookupType(std::string resourceName,
                                       std::string lookupName,
                                       std::string lookupValue) const;

    /**
     * Returns a list of MetadataObjects from a resource name.
     *
     * @param resourceName A resource name
     */
    MetadataObjectList GetAllObjects(std::string resourceName) const;

    /**
     * Returns a list of MetadataObjects from a resource.
     *
     * @param metadataResource a MetadataResource object
     */
    MetadataObjectList GetAllObjects(MetadataResource * metadataResource)
        const;

    /**
     * Returns the requested SearchHelp given the resource name and
     * searchHelpID.
     *
     * @param resourceName A resource name
     * @param searchHelpID The search help ID
     * @return A MetadataSearchHelp
     */
    MetadataSearchHelp* GetSearchHelp(std::string resourceName,
                                      std::string searchHelpID) const;
    
  private:
    void InitSystem();

    MetadataFinderPtr mFinder;
    MetadataSystem * mSystem;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

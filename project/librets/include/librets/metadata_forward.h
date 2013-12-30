/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 * Portions Copyright (C) 2014 Real Estate Standards Organziation
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
#ifndef LIBRETS_METADATA_FORWARD_H
#define LIBRETS_METADATA_FORWARD_H
/**
 * @file metadata_forward.h
 * (Internal) Contains the metadata protocol forward
 * declarations.
 */
/// @cond MAINTAINER


#include <boost/shared_ptr.hpp>

namespace librets {

class RetsMetadata;
/** Smart pointer to RetsMetadata. */
typedef boost::shared_ptr<RetsMetadata> RetsMetadataPtr;

class MetadataElement;
/** Smart pointer to MetadataElement. */
typedef boost::shared_ptr<MetadataElement> MetadataElementPtr;

class MetadataSystem;
/** Smart pointer to MetadataSystem. */
typedef boost::shared_ptr<MetadataSystem> MetadataSystemPtr;

class MetadataResource;
/** Smart pointer to MetadataResource. */
typedef boost::shared_ptr<MetadataResource> MetadataResourcePtr;

class MetadataClass;
/** Smart pointer to MetadataClass */
typedef boost::shared_ptr<MetadataClass> MetadataClassPtr;

class MetadataTable;
/** Smart pointer to MetadataTable. */
typedef boost::shared_ptr<MetadataTable> MetadataTablePtr;

class MetadataUpdate;
/** Smart pointer to MetadataUpdate. */
typedef boost::shared_ptr<MetadataUpdate> MetadataUpdatePtr;

class MetadataUpdateType;
/** Smart pointer to MetadataUpdateType. */
typedef boost::shared_ptr<MetadataUpdateType> MetadataUpdateTypePtr;

class MetadataObject;
/** Smart pointer to MetadataObject. */
typedef boost::shared_ptr<MetadataObject> MetadataObjectPtr;

class MetadataSearchHelp;
/** Smart pointer to MetadataSearchHelp. */
typedef boost::shared_ptr<MetadataSearchHelp> MetadataSearchHelpPtr;

class MetadataEditMask;
/** Smart pointer to MetadataEditMask. */
typedef boost::shared_ptr<MetadataEditMask> MetadataEditMaskPtr;

class MetadataLookup;
/** Smart pointer to MetadataLookup. */
typedef boost::shared_ptr<MetadataLookup> MetadataLookupPtr;

class MetadataLookupType;
/** Smart pointer to MetadataLookupType. */
typedef boost::shared_ptr<MetadataLookupType> MetadataLookupTypePtr;

class MetadataUpdateHelp;
/** Smart pointer to MetadataUpdateHelp. */
typedef boost::shared_ptr<MetadataUpdateHelp> MetadataUpdateHelpPtr;

class MetadataValidationLookup;
/** Smart pointer to MetadataValidationLookup. */
typedef boost::shared_ptr<MetadataValidationLookup>
    MetadataValidationLookupPtr;

class MetadataValidationLookupType;
/** Smart pointer to MetadataValidationLookupType. */
typedef boost::shared_ptr<MetadataValidationLookupType>
    MetadataValidationLookupTypePtr;

class MetadataValidationExternal;
/** Smart pointer to MetadataValidationExternal. */
typedef boost::shared_ptr<MetadataValidationExternal>
    MetadataValidationExternalPtr;

class MetadataValidationExternalType;
/** Smart pointer to MetadataValidationExternalType. */
typedef boost::shared_ptr<MetadataValidationExternalType>
    MetadataValidationExternalTypePtr;

class MetadataValidationExpression;
/** Smart pointer to MetadataValidationExpression. */
typedef boost::shared_ptr<MetadataValidationExpression>
    MetadataValidationExpressionPtr;

class MetadataForeignKey;
/** Smart pointer to MetadataForeignKey. */
typedef boost::shared_ptr<MetadataForeignKey> MetadataForeignKeyPtr;

class MetadataElementCollector;
/** Smart pointer to MetadataElementCollector */
typedef boost::shared_ptr<MetadataElementCollector>
    MetadataElementCollectorPtr;

class MetadataLoader;
/** Smart pointer to MetadataLoader */
typedef boost::shared_ptr<MetadataLoader> MetadataLoaderPtr;

class MetadataFinder;
/** Smart pointer to MetadataFinder */
typedef boost::shared_ptr<MetadataFinder> MetadataFinderPtr;

class IncrementalMetadataFinder;
/** Smart pointer to IncrementalMetadataFinder */
typedef boost::shared_ptr<IncrementalMetadataFinder>
    IncrementalMetadataFinderPtr;

class XmlMetadataParser;
/** Smart pointer to XmlMetadataParser */
typedef boost::shared_ptr<XmlMetadataParser> XmlMetadataParserPtr;

class XmlMetadataElementFactory;
/** Smart pointer to XmlMetadataElementFactory */
typedef boost::shared_ptr<XmlMetadataElementFactory>
    XmlMetadataElementFactoryPtr;

class DefaultMetadataElementFactory;
/** Smart pointer to DefaultMetadataElementFactory */
typedef boost::shared_ptr<DefaultMetadataElementFactory>
    DefaultMetadataElementFactoryPtr;

class DefaultMetadataCollector;
/** Smart pointer to DefaultMetadataCollector */
typedef boost::shared_ptr<DefaultMetadataCollector>
    DefaultMetadataCollectorPtr;

class RetsUnknownMetadataException;
    
class MetadataColumnGroup;
/** Smart pointer to MetadataColumnGroup RETS 1.8 */
typedef boost::shared_ptr<MetadataColumnGroup> 
    MetadataColumnGroupPtr;
    
class MetadataColumnGroupNormalization;
/** Smart pointer to MetadataColumnGroupNormalization RETS 1.8 */
typedef boost::shared_ptr<MetadataColumnGroupNormalization> 
    MetadataColumnGroupNormalizationPtr;
    
class MetadataColumnGroupTable;
/** Smart pointer to MetadataColumnGroupTable RETS 1.8 */
typedef boost::shared_ptr<MetadataColumnGroupTable> 
    MetadataColumnGroupTablePtr;
    
class MetadataColumnGroupControl;
/** Smart pointer to MetadataColumnGroupControl RETS 1.8 */
typedef boost::shared_ptr<MetadataColumnGroupControl> 
    MetadataColumnGroupControlPtr;
    
class MetadataColumnGroupSet;
/** Smart pointer to MetadataColumnGroupSet RETS 1.8 */
typedef boost::shared_ptr<MetadataColumnGroupSet> 
    MetadataColumnGroupSetPtr;
    
class MetadataFilter;
/** Smart pointer to MetadataFilter RETS 1.8 */
typedef boost::shared_ptr<MetadataFilter> 
    MetadataFilterPtr;

class MetadataFilterType;
/** Smart pointer to MetadataFilter RETS 1.8 */
typedef boost::shared_ptr<MetadataFilterType> 
    MetadataFilterTypePtr;

};

/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

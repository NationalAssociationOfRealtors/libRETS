/*
 * Copyright (C) 2005 National Association of REALTORS(R)
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
#ifndef LIBRETS_DEFAULT_METADATA_ELEMENT_FACTORY_H
#define LIBRETS_DEFAULT_METADATA_ELEMENT_FACTORY_H
/** 
 * @file DefaultMetadataElementFactory.h
 * Contains the DefaultMetadataElementFactory class declaration.
 */
/// @cond MAINTAINER

#include <map>
#include "librets/XmlMetadataElementFactory.h"
#include "librets/MetadataElement.h"

namespace librets {

/**
 * (Internal) DefaultMetadataElementFactory is the default implementation of
 * XmlMetadataElementFactory.
 */
class DefaultMetadataElementFactory : public XmlMetadataElementFactory
{
  public:
    DefaultMetadataElementFactory();

    virtual ~DefaultMetadataElementFactory();

    /**
     * Set the error handler.
     * @param errorHandler A pointer to the RetsErrorHandler that will be called
     * on errors.
     */
    virtual void SetErrorHandler(RetsErrorHandler * errorHandler);

    /**
     * Create a metadata element.
     * @param startElementEvent A pointer to the RetsXmlStartElementEvent that begins
     * this metadata element.
     */ 
    virtual MetadataElementPtr CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent);

    /**
     * Locate the metadata element for the given type and resource/class.
     * @param type The metadata type such as SYSTEM or TABLE.
     * @param level A string containing the reosurce/class to find.
     */
    MetadataElementListPtr Find(MetadataElement::MetadataType type,
                                std::string level);

  private:
    typedef MetadataElementPtr
        (DefaultMetadataElementFactory::* CreatorPointer)
        (RetsXmlStartElementEventPtr startElementEvent);
    typedef std::map<std::string, const char **> AttributeMap;

    typedef std::map<std::string, CreatorPointer> CreatorMap;
    typedef std::map<std::string, MetadataElementListPtr> LevelMap;
    typedef boost::shared_ptr<LevelMap> LevelMapPtr;
    typedef std::map<MetadataElement::MetadataType, LevelMapPtr>
        TypeMap;
    
    void AddMappings(std::string name, CreatorPointer creator,
                     const char ** attributes);

    MetadataElementPtr CreateSystem(
        RetsXmlStartElementEventPtr startElementEvent);
    
    MetadataElementPtr CreateResource(
        RetsXmlStartElementEventPtr startElementEvent);
    
    MetadataElementPtr CreateClass(
        RetsXmlStartElementEventPtr startElementEvent);
    
    MetadataElementPtr CreateTable(
        RetsXmlStartElementEventPtr startElementEvent);
    
    MetadataElementPtr CreateUpdate(
        RetsXmlStartElementEventPtr startElementEvent);
    
    MetadataElementPtr CreateUpdateType(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateUpdateHelp(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateObject(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateSearchHelp(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateEditMask(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateLookup(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateLookupType(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateValidationLookup(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateValidationLookupType(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateValidationExternal(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateValidationExternalType(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementPtr CreateValidationExpression(
        RetsXmlStartElementEventPtr startElementEvent);
    
    MetadataElementPtr CreateForeignKey(
        RetsXmlStartElementEventPtr startElementEvent);
    
    void AddElement(MetadataElementPtr element, std::string level);

    std::string LevelBuilder(RetsXmlStartElementEventPtr startEvent,
                             const char ** attributes);

    TypeMap mTypeMap;
    CreatorMap mCreatorMap;
    AttributeMap mAttributeMap;
    RetsErrorHandler * mErrorHandler;

    static const char * SYSTEM_ATTRIBUTES[];
    static const char * RESOURCE_ATTRIBUTES[];
    static const char * CLASS_ATTRIBUTES[];
    static const char * TABLE_ATTRIBUTES[];
    static const char * UPDATE_ATTRIBUTES[];
    static const char * UPDATE_TYPE_ATTRIBUTES[];
    static const char * UPDATE_HELP_ATTRIBUTES[];
    static const char * OBJECT_ATTRIBUTES[];
    static const char * SEARCH_HELP_ATTRIBUTES[];
    static const char * EDIT_MASK_ATTRIBUTES[];
    static const char * LOOKUP_ATTRIBUTES[];
    static const char * LOOKUP_TYPE_ATTRIBUTES[];
    static const char * VALIDATION_LOOKUP_ATTRIBUTES[];
    static const char * VALIDATION_LOOKUP_TYPE_ATTRIBUTES[];
    static const char * VALIDATION_EXTERNAL_ATTRIBUTES[];
    static const char * VALIDATION_EXTERNAL_TYPE_ATTRIBUTES[];
    static const char * VALIDATION_EXPRESSION_ATTRIBUTES[];
    static const char * FOREIGN_KEY_ATTRIBUTES[];
};

};

#endif
/// @endcond

/* Local Variables: */
/* mode: c++ */
/* End: */

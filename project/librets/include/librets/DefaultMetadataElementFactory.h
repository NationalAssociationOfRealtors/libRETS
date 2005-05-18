#ifndef LIBRETS_DEFAULT_METADATA_ELEMENT_FACTORY_H
#define LIBRETS_DEFAULT_METADATA_ELEMENT_FACTORY_H

#include <map>
#include "librets/XmlMetadataElementFactory.h"
#include "librets/MetadataElement.h"

namespace librets {

class DefaultMetadataElementFactory : public XmlMetadataElementFactory
{
  public:
    DefaultMetadataElementFactory();

    virtual ~DefaultMetadataElementFactory();

    virtual MetadataElementPtr CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent);

    MetadataElementListPtr Find(MetadataElement::MetadataType type,
                                std::string level);

  private:
    typedef MetadataElementPtr
        (DefaultMetadataElementFactory::* CreatorPointer)
        (RetsXmlStartElementEventPtr startElementEvent);
    typedef std::map<std::string, char **> AttributeMap;

    typedef std::map<std::string, CreatorPointer> CreatorMap;
    typedef std::map<std::string, MetadataElementListPtr> LevelMap;
    typedef boost::shared_ptr<LevelMap> LevelMapPtr;
    typedef std::map<MetadataElement::MetadataType, LevelMapPtr>
        TypeMap;
    
    void AddMappings(std::string name, CreatorPointer creator,
                     char ** attributes);

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

    void AddElement(MetadataElementPtr element, std::string level);

    std::string LevelBuilder(RetsXmlStartElementEventPtr startEvent,
                             char ** attributes);

    TypeMap mTypeMap;
    CreatorMap mCreatorMap;
    AttributeMap mAttributeMap;

    static char * SYSTEM_ATTRIBUTES[];
    static char * RESOURCE_ATTRIBUTES[];
    static char * CLASS_ATTRIBUTES[];
    static char * TABLE_ATTRIBUTES[];
    static char * UPDATE_ATTRIBUTES[];
    static char * UPDATE_TYPE_ATTRIBUTES[];
    static char * UPDATE_HELP_ATTRIBUTES[];
    static char * OBJECT_ATTRIBUTES[];
    static char * SEARCH_HELP_ATTRIBUTES[];
    static char * EDIT_MASK_ATTRIBUTES[];
    static char * LOOKUP_ATTRIBUTES[];
    static char * LOOKUP_TYPE_ATTRIBUTES[];
    static char * VALIDATION_LOOKUP_ATTRIBUTES[];
    static char * VALIDATION_LOOKUP_TYPE_ATTRIBUTES[];
    static char * VALIDATION_EXTERNAL_ATTRIBUTES[];
    static char * VALIDATION_EXTERNAL_TYPE_ATTRIBUTES[];
    static char * VALIDATION_EXPRESSION_ATTRIBUTES[];
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

#ifndef LIBRETS_XML_METADATA_PARSER_H
#define LIBRETS_XML_METADATA_PARSER_H

#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/metadata_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class XmlMetadataParser : public RetsObject
{
  public:
    XmlMetadataParser(MetadataElementCollectorPtr elementCollector);

    void SetElementFactory(XmlMetadataElementFactoryPtr elementFactory);

    void Parse(istreamPtr inputStream);

  private:
    void HandleSystemMetadata(RetsXmlStartElementEventPtr metadtaEvent);
    void HandleOtherMetadata(RetsXmlStartElementEventPtr metadtaEvent);

    MetadataElementCollectorPtr mElementCollector;
    XmlMetadataElementFactoryPtr mElementFactory;
    RetsXmlParserPtr mXmlParser;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

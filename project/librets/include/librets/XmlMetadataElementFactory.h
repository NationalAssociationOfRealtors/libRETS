#ifndef LIBRETS_XML_METADATA_ELEMENT_FACTORY_H
#define LIBRETS_XML_METADATA_ELEMENT_FACTORY_H

#include "librets/xml_forward.h"
#include "librets/metadata_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class XmlMetadataElementFactory : public RetsObject
{
  public:
    virtual ~XmlMetadataElementFactory();

    virtual MetadataElementPtr CreateMetadataElement(
        RetsXmlStartElementEventPtr startElementEvent) = 0;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

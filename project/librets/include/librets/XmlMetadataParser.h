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
    void HandleSystemMetadata(RetsXmlStartElementEventPtr metadataEvent);
    void HandleOtherMetadata(RetsXmlStartElementEventPtr metadataEvent);
    void HandleColumns();
    void HandleData(RetsXmlStartElementEventPtr metadataEvent);

    MetadataElementCollectorPtr mElementCollector;
    XmlMetadataElementFactoryPtr mElementFactory;
    RetsXmlParserPtr mXmlParser;
    StringVector columns;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

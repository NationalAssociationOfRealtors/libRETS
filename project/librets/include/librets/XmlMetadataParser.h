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
/** 
 * @file XmlMetadataParser.h
 * (Internal) Contains the XML Parser for metadata interface class for use with libexpat.
 */
/// @cond MAINTAINER

#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/metadata_forward.h"
#include "librets/error_forward.h"
#include "librets/RetsObject.h"
#include "librets/EncodingType.h"

namespace librets {
/**
 * (Internal) XmlMetadataParser is the primary class for parsing metadata.
 */
class XmlMetadataParser : public RetsObject
{
  public:
    XmlMetadataParser(MetadataElementCollectorPtr elementCollector,
                      RetsErrorHandler * errorHandler);

    void SetElementFactory(XmlMetadataElementFactoryPtr elementFactory);

    /**
     * Set the data encoding flag to allow for parsing of extended
     * characters by Expat.  RETS is officially US-ASCII, but this
     * will allow a work around for servers that haven't properly
     * sanitized their data.
     *
     * @param encoding RETS_XML_DEFAULT_ENCODING,
     * RETS_XML_ISO_ENCODING or RETS_XML_UTF8_ENCODING.
     */
    void SetEncoding(EncodingType encoding);

    /**
     * Set the error handler.
     * @param errorHandler The RetsErrorHandler pointer
     */
    void SetErrorHandler(RetsErrorHandler * errorHandler);

    /**
     * Parse the input stream for metadata.
     * @param inputStream The stream from which the metadata is to be parsed.
     */
    void Parse(istreamPtr inputStream);

  private:
    /**
     * Handle the &lt;METADATA-SYSTEM&gt; element.
     * @param metadataEvent A pointer to the RetsXmlStartElementEvent.
     */
    void HandleSystemMetadata(RetsXmlStartElementEventPtr metadataEvent);
    
    /**
     * Handles elements such as &lt;COLUMN&gt; and &lt;DATA&gt;
     * @param metadataEvent A pointer to the RetsXmlEvent.
     */
    void HandleOtherMetadata(RetsXmlStartElementEventPtr metadataEvent);
    
    /**
     * Parse the column header detail.
     */
    void HandleColumns();
    
    /**
     * Parse the row data.
     * @param metadataEvent A pointer to the RetsXmlStartElementEvent.
     */
    void HandleData(RetsXmlStartElementEventPtr metadataEvent);

    MetadataElementCollectorPtr mElementCollector;
    XmlMetadataElementFactoryPtr mElementFactory;
    RetsErrorHandler * mErrorHandler;
    RetsXmlParserPtr mXmlParser;
    StringVector mColumns;

    EncodingType mEncoding;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

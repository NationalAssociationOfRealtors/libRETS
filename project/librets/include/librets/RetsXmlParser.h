#ifndef LIBRETS_RETS_XML_PARSER_H
#define LIBRETS_RETS_XML_PARSER_H

#include "librets/std_forward.h"
#include "librets/xml_forward.h"

namespace librets {

class RetsXmlParser
{
  public:
    static RetsXmlParserPtr CreateDefault(istreamPtr inputStream);

    virtual ~RetsXmlParser();

    virtual bool HasNext() = 0;

    /**
     * Returns next XML event, blocking if necessary.
     *
     * @return Next XML event
     * @throw RetsException
     */
    virtual RetsXmlEventPtr GetNextEvent() = 0;

    RetsXmlEventPtr GetNextSkippingEmptyText();
    
    RetsXmlStartElementEventPtr AssertNextIsStartEvent();

    RetsXmlEndElementEventPtr AssertNextIsEndEvent();

    RetsXmlTextEventPtr AssertNextIsTextEvent();
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

#ifndef LIBRETS_XML_FORWARD_H
#define LIBRETS_XML_FORWARD_H

#include <boost/shared_ptr.hpp>

namespace librets {

class RetsXmlParser;
typedef boost::shared_ptr<RetsXmlParser> RetsXmlParserPtr;

class RetsXmlEvent;
typedef boost::shared_ptr<RetsXmlEvent> RetsXmlEventPtr;

class RetsXmlAttribute;
typedef boost::shared_ptr<RetsXmlAttribute> RetsXmlAttributePtr;

class RetsXmlStartElementEvent;
typedef boost::shared_ptr<RetsXmlStartElementEvent>
    RetsXmlStartElementEventPtr;

class RetsXmlEndElementEvent;
typedef boost::shared_ptr<RetsXmlEndElementEvent> RetsXmlEndElementEventPtr;

class RetsXmlTextEvent;
typedef boost::shared_ptr<RetsXmlTextEvent> RetsXmlTextEventPtr;

class ExpatXmlParser;
typedef boost::shared_ptr<ExpatXmlParser> ExpatXmlParserPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

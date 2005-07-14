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

#ifndef LIBRETS_XML_FORWARD_H
#define LIBRETS_XML_FORWARD_H

#include <boost/shared_ptr.hpp>
#include <vector>

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
    
/** A vector of RetsXmlEvent objects. */
typedef std::vector<RetsXmlEventPtr> RetsXmlEventList;
/** A smart pointer to RetsXmlEventList. */
typedef boost::shared_ptr<RetsXmlEventList> RetsXmlEventListPtr;

class ExpatXmlParser;
typedef boost::shared_ptr<ExpatXmlParser> ExpatXmlParserPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

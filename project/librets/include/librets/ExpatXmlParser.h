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
#ifndef LIBRETS_EXPAT_XML_PARSER_H
#define LIBRETS_EXPAT_XML_PARSER_H

#include <istream>
#include <list>
#include <expat.h>
#include "librets/http_forward.h"
#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/RetsXmlParser.h"

namespace librets {

class ExpatXmlParser : public RetsXmlParser
{
  public:
    ExpatXmlParser(istreamPtr inputStream, const char *encoding = "US-ASCII");
    ExpatXmlParser(std::string inputString, const char *encoding = "US-ASCII");
    ExpatXmlParser(RetsHttpClientPtr httpClient, istreamPtr inputStream, const char *encoding = "US-ASCII");

    virtual ~ExpatXmlParser();

    virtual bool HasNext();

    virtual RetsXmlEventPtr GetNextEvent();

  private:
    typedef std::list<RetsXmlEventPtr> XmlEventList;

    void init(istreamPtr inputStream, const char *encoding = "US-ASCII");

    RetsXmlEventPtr GetNextEventWithoutCoalescing();
    void CoalesceTextEvents(RetsXmlTextEventPtr textEvent);

    static void StartElement(void * userData, const char * name,
                             const char **atts);
    static void EndElement(void * userData, const char * name);

    static void CharacterData(void * userData, const XML_Char * s,
                              int len);
    
    int GetCurrentLineNumber() const;
    int GetCurrentColumnNumber() const;

    XmlEventList mEvents;
    istreamPtr mInputStream;
    bool mIsDone;
    XML_Parser mParser;
    RetsHttpClientPtr mHttpClient;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

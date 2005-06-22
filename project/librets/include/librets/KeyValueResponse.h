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
#ifndef LIBRETS_KEY_VALUE_RESPONSE_H
#define LIBRETS_KEY_VALUE_RESPONSE_H

#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class KeyValueResponse : public RetsObject
{
  public:
    virtual ~KeyValueResponse();

    void Parse(istreamPtr inputStream);

    std::string GetValue(std::string key) const;

  protected:
    virtual void ParsingFinished();

  private:
    static RetsXmlEventPtr getNextSkippingEmptyText(
        RetsXmlParserPtr xmlParser);

    static RetsXmlStartElementEventPtr assertNextIsStart(
        RetsXmlParserPtr xmlParser);

    static RetsXmlEndElementEventPtr assertNextIsEnd(
        RetsXmlParserPtr xmlParser);

    static RetsXmlTextEventPtr assertNextIsText(RetsXmlParserPtr xmlParser);
    
    static void assertEquals(const std::string & expected,
                             const std::string & actual);

    StringMap mValues;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

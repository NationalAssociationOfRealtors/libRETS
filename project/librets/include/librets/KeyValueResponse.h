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

#ifndef LIBRETS_RETS_XML_START_ELEMENT_EVENT_H
#define LIBRETS_RETS_XML_START_ELEMENT_EVENT_H

#include "librets/RetsXmlEvent.h"
#include "librets/xml_forward.h"
#include <ostream>
#include <vector>
#include <map>

namespace librets {

typedef std::vector<RetsXmlAttributePtr> RetsXmlAttributeList;
typedef boost::shared_ptr<RetsXmlAttributeList> RetsXmlAttributeListPtr;

class RetsXmlStartElementEvent : public RetsXmlEvent
{
  public:
    RetsXmlStartElementEvent();
    
    virtual ~RetsXmlStartElementEvent();

    virtual Type GetType() const;

    void SetName(std::string name);

    std::string GetName() const;

    void AddAttribute(RetsXmlAttributePtr attribute);

    void AddAttribute(std::string name, std::string value);

    RetsXmlAttributeListPtr GetAttributes();

    std::string GetAttributeValue(std::string name);

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * rhs) const;

  private:
    typedef std::map<std::string, RetsXmlAttributePtr> AttributeMap;

    std::string mName;

    RetsXmlAttributeListPtr mAttributes;

    AttributeMap mAttributesByName;
};

};


#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

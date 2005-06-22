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

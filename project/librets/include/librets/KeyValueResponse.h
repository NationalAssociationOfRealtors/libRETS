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
/**
 * @file KeyValueResponse.h 
 * (Internal) Contains the KeyValueResponse class definition.
 */
 
 /// @cond MAINTAINER
 
#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/RetsObject.h"
#include "librets/RetsVersion.h"

namespace librets {

/**
 * KeyValueResponse is a class that parses a stream in the form of 
 * <code>key=value</code>, saves and enables access to the results.
 */
class KeyValueResponse : public RetsObject
{
  public:
    virtual ~KeyValueResponse();

    /**
     * Parse the input stream, converting the data into keys and values.
     * @param inputStream A pointer to the input stream.
     * @param retsVersion The RETS version.
     */
    void Parse(istreamPtr inputStream, RetsVersion retsVersion);

    /**
     * For a given key, return the associated value. If the key is not found
     * and empty string is returned.
     * @param key A string representing the key whose value is to be returned.
     * @return A string containing the value.
     */
    std::string GetValue(std::string key) const;

  protected:
    virtual RetsXmlTextEventPtr GetBodyEvent(RetsXmlEventListPtr eventList,
                                             RetsVersion retsVersion) = 0;
    
    RetsXmlTextEventPtr GetBodyEventFromStandardResponse(
        RetsXmlEventListPtr eventList);
    
    RetsXmlTextEventPtr GetBodyEventFromResponseWithNoRetsResponse(
        RetsXmlEventListPtr eventList);

    RetsXmlTextEventPtr GetBodyEventFromShortResponse(
        RetsXmlEventListPtr eventList);
    
    virtual void ParsingFinished();
    
    static void AssertEquals(const std::string & expected,
                             const std::string & actual);
    
    static void AssertEquals(int expected, int actual);
    
    static void AssertEventListSize(int expected,
                                    RetsXmlEventListPtr eventList);

  private:
    void ParseBody(std::string body);
    
    StringMap mValues;
};

};

/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
#ifndef LIBRETS_SERVERINFO_RESPONSE_H
#define LIBRETS_SERVERINFO_RESPONSE_H
/** 
 * @file ServerInfoResponse.h
 * Contains the ServerInformationResponse class definition.
 */
 
#include <string>
#include <vector>
#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/RetsObject.h"
#include "librets/RetsSession.h"

namespace librets {

/**
 * ServerInformationResponse is the API that allows access to the RETS 1.7
 * SERVERINFORMATION transaction results.
 */
class ServerInformationResponse : public virtual RetsObject
{
  public:
    /**
     * Default constructor.
     */
    ServerInformationResponse();

    virtual ~ServerInformationResponse();
    
    /// @cond MAINTAINER
    /**
     * Parse the Server Information Response.
     * @param inputStream The stream containing the response to be parsed.
     * @throw RetsReplyException
     */
    void Parse(istreamPtr inputStream);
    /// @endcond
    
    /**
     * Returns the parameter names.
     *
     * @return The parameter names
     */
    const StringVector GetParameters();

    /**
     * Returns the resource name of a parameter as a string.
     *
     * @param parameterName parameter name
     * @return string value of resource name
     */
    std::string GetResource(std::string parameterName);
    
    /**
     * Returns the class name of a parameter as a string.
     *
     * @param parameterName parameter name
     * @return string value of class name
     */
    std::string GetClass(std::string parameterName);
    
    /**
     * Returns the value of a parameter as a string.
     *
     * @param parameterName parameter name
     * @return string value of parameter
     */
    std::string GetValue(std::string parameterName);

    /**
     * Set the data encoding flag to allow for parsing of extended
     * characters by Expat.  RETS is officially US-ASCII, but this
     * will allow a work around for servers that haven't properly
     * sanitized their data.
     *
     * @param encoding Either RETS_XML_DEFAULT_ENCODING or
     * RETS_XML_ISO_ENCODING.
     */
    void SetEncoding(EncodingType encoding);

    /**
     * Get the current value for the data encoding flag.
     *
     * @return string value of encoding flag.
     */
    EncodingType GetEncoding();
    
    /**
     * Returns the RETS-STATUS ReplyCode.
     *
     * @return int value of ReplyCode
     */
    int GetReplyCode();
    
    /**
     * Returns the RETS-STATUS ReplyText.
     *
     * @return string value of ReplyText.
     */
    std::string GetReplyText();

    /// @cond MAINTAINER
    /**
     * Set the input stream for Parse.
     *
     * @param inputStream Input Stream 
     */
    void SetInputStream(istreamPtr inputStream);
    /// @endcond
    
  private:
    
    StringMap mValues;
    StringMap mResources;
    StringMap mClasses;
    StringVector mParameters;

    EncodingType mEncoding;
    int mReplyCode;
    std::string mReplyText;

    istreamPtr mParseInputStream;
    ExpatXmlParserPtr mXmlParser;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

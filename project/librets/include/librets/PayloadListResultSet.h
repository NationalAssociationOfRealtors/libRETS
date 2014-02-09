/*
 * Copyright (C) 2005-2014 National Association of REALTORS(R)
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
#ifndef LIBRETS_PAYLOAD_LIST_RESULT_SET_H
#define LIBRETS_PAYLOAD_LIST_RESULT_SET_H
/**
 * @file PayloadListResultSet.h
 * Contains the PayloadListResultSet class definition.
 */

#include <string>
#include <vector>
#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/RetsObject.h"
#include "librets/RetsSession.h"

namespace librets {

/**
 * RETS 1.8
 * PayloadListResultSet contains the API that is used to handle the
 * response to the RETS GETPAYLOADLIST transaciton.
 */
class PayloadListResultSet : public virtual RetsObject
{
  public:
    /**
     * Default constructor.
     */
    PayloadListResultSet();

    virtual ~PayloadListResultSet();

    /// @cond MAINTAINER
    /**
     * Parse the result sent back from the client.
     * @param inputStream The input stream containing the data to be parsed.
     * @throw RetsReplyException
     */
    void Parse(istreamPtr inputStream);
    /// @endcond
    
    /**
     * Returns true if there are more results.  This may block waiting
     * for data from the RETS server.
     *
     * HasNext() has the side effect of incrementing the result set to
     * the next result.  After a query, this must be called before you
     * can access the first result row.
     *
     * @return If there is a next item or not.
     */
    bool HasNext();

    /**
     * Returns the column names.
     *
     * @return A string vector containing the column names
     */
    const StringVector GetColumns();

    /**
     * Returns the value of a column as a string.
     *
     * @param columnIndex column index
     * @return string value of column
     */
    std::string GetString(int columnIndex);

    /**
     * Returns the value of a column as a string.
     *
     * @param columnName column name
     * @return string value of column
     */
    std::string GetString(std::string columnName);

    /**
     * Set the data encoding flag to allow for parsing of extended
     * characters by Expat.  RETS is officially US-ASCII, but this
     * will allow a work around for servers that haven't properly
     * sanitized their data.
     *
     * @param encoding Either RETS_XML_DEFAULT_ENCODING,
     * RETS_XML_ISO_ENCODING or RETS_XML_UTF8_ENCODING.
     */
    void SetEncoding(EncodingType encoding);

    /// @cond MAINTAINER
    /**
     * Enable or disable caching. Disabling caching only works in
     * streaming mode. It is up to the caller to verify that the
     * operational mode is proper.
     * @param enable  A boolean that if TRUE enables caching.
     */
    void SetCaching(bool enable);
    /// @endcond
    
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

    /**
     * Set the input stream for Parse.
     *
     * @param inputStream Input Stream 
     */
    void SetInputStream(istreamPtr inputStream);
   
    /**
     * Sets the raw RETS data contained in the BinaryData object 
     * as the input to be parsed for the search.
     * 
     * @param binaryData A BinaryData object containing the raw RETS data.
     */
    void SetInputData(BinaryData binaryData);
    
    /**
     * Get the Class from the RETSPayloadList element
     *
     * @return string value of Class
     */
    std::string GetPayloadClass();
    
    /**
     * Get the Date attribute from the RETSPayloadList element
     *
     * @return string value of Date
     */
    std::string GetPayloadDate();
    
    /**
     * Get the Resource attribute from the RETSPayloadList element
     *
     * @return string value of Resource
     */
    std::string GetPayloadResource();
    
    /**
     * Get the Version attribute from the RETSPayloadList element
     *
     * @return string value of the Version
     */
    std::string GetPayloadVersion();
    
  private:
    typedef std::vector<StringVectorPtr> RowData;
    typedef std::map<std::string, StringVector::size_type> ColumnToIndexMap;
    void FixCompactArray(StringVector & compactVector, std::string context);
    /**
     * Parse the XML. An input stream must have already been
     * established. This method should only be called internally.
     *
     * @return Returns TRUE if a row was successfully parsed.
     */
    bool Parse();


    StringVectorPtr mColumns;
    ColumnToIndexMap mColumnToIndex;
    RowData mRows;
    long mNextRow;
    StringVectorPtr mCurrentRow;
    EncodingType mEncoding;
    int mReplyCode;
    std::string mReplyText;

    istreamPtr mParseInputStream;
    ExpatXmlParserPtr mXmlParser;
    bool mCaching;
    std::string mDelimiter;
    
    std::string mClass;
    std::string mDate;
    std::string mResource;
    std::string mVersion;

};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

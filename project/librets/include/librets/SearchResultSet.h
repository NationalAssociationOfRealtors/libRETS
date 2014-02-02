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
#ifndef LIBRETS_SEARCH_RESULT_SET_H
#define LIBRETS_SEARCH_RESULT_SET_H
/**
 * @file SearchResultSet.h
 * Contains the SearchResultSet class definition.
 */

#include <string>
#include <vector>
#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/RetsObject.h"
#include "librets/RetsSession.h"

namespace librets {

/**
 * SearchResultSet contains the API that is used to handle the
 * response to the RETS SEARCH transaciton.
 */
class SearchResultSet : public virtual RetsObject
{
  public:
    /**
     * Default constructor.
     */
    SearchResultSet();

    virtual ~SearchResultSet();

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
     * Returns the number of rows in the result set, or -1 if not
     * known.  The number of rows is only available if it was
     * specified in the SearchRequest.
     *
     * @return An integer representing the number of rows, or -1
     */
    int GetCount();

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
     * Indicate whether the MAXROWS tag has been seen.
     *
     * @return Boolean value
     */
    bool HasMaxRows();
    
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
    
#ifdef DOXYGEN
    /**
     * Inject the Raw XML response from a search into the <code>SearchResultSet</code>
     * as an array. This API is for .NET and Java only.
     *
     * @param bytes  A byte array containing the raw search XML.
     */
    public void SetDataAsArray(byte [] bytes);

    /**
     * Inject the Raw XML response from a search into the <code>SearchResultSet</code>
     * as a string. This API is for PHP only.
     *
     * @param bytes  A string containing the raw search XML.
     */
    public void SetDataAsString(std::string bytes);
#endif

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
    bool parseRESORecord(std::string level);


    int mCount;
    StringVectorPtr mColumns;
    ColumnToIndexMap mColumnToIndex;
    RowData mRows;
    long mNextRow;
    StringVectorPtr mCurrentRow;
    EncodingType mEncoding;
    bool mMaxRows;
    int mReplyCode;
    std::string mReplyText;

    istreamPtr mParseInputStream;
    ExpatXmlParserPtr mXmlParser;
    std::string mDelimiter;
    bool mCaching;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

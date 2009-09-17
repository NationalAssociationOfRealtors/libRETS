/*
 * Copyright (C) 2009 National Association of REALTORS(R)
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
#ifndef LIBRETS_UPDATE_RESULT_H
#define LIBRETS_UPDATE_RESULT_H
/**
 * @file UpdateResponse.h
 * Contains the UpdateResponse class definition.
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
class UpdateResponse : public virtual RetsObject
{
  public:
    /**
     * Default constructor.
     */
    UpdateResponse();

    virtual ~UpdateResponse();

    /**
     * Returns the column names.
     *
     * @return A string vector containing the column names
     */
    const StringVector GetColumns();
  
    /**
     * Get the current value for the data encoding flag.
     *
     * @return string value of encoding flag.
     */
    EncodingType GetEncoding();
    
    /**
     * Get the field name of the current error.
     *
     * @return string value of the field system name.
     */
    std::string GetErrorFieldName();
    
    /**
     * Get the error number for the current error.
     *
     * @return int value of the error.
     */
    int GetErrorNumber();
    
    /**
     * Get the offset of the error.
     *
     * @return int containing the offset of the error if known.
     * 0 indicates unknown.
     */
    int GetErrorOffset();
    
    /**
     * Get the text associated with the error.
     *
     * @return string containing the error text.
     */
    std::string GetErrorText();
    
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
     * Get the field name of the current warning.
     *
     * @return string value of the field system name.
     */
    std::string GetWarningFieldName();
    
    /**
     * Get the error number for the current warning.
     *
     * @return int value of the warning.
     */
    int GetWarningNumber();
    
    /**
     * Get the offset of the warning.
     *
     * @return int containing the offset of the warning if known.
     * 0 indicates unknown.
     */
    int GetWarningOffset();
    
    /**
     * Get the response required indicator.
     *
     * @return int containing the value of the response required field.
     */
    int GetWarningResponseRequired();
    
    /**
     * Get the text associated with the warning.
     *
     * @return string containing the warning text.
     */
    std::string GetWarningText();
    
    /**
     * Returns true if there are more results.  This may block waiting
     * for data from the RETS server.
     *
     * HasNext() has the side effect of incrementing the result set to
     * the next result.  After an update, this must be called before you
     * can access the first result row.
     *
     * @return If there is a next item or not.
     */
    bool HasNext();
    
    /**
     * Returns true if there are more results.  This may block waiting
     * for data from the RETS server.
     *
     * HasNext() has the side effect of incrementing the result set to
     * the next result.  After an update, this must be called before you
     * can access the first result row.
     *
     * @return If there is a next item or not.
     */
    
    bool HasNextError();
    /**
     * Returns true if there are more results.  This may block waiting
     * for data from the RETS server.
     *
     * HasNext() has the side effect of incrementing the result set to
     * the next result.  After an update, this must be called before you
     * can access the first result row.
     *
     * @return If there is a next item or not.
     */
    bool HasNextWarning();
    
    /// @cond MAINTAINER
    /**
     * Parse the result sent back from the client.
     * @param inputStream The input stream containing the data to be parsed.
     * @throw RetsReplyException
     */
    void Parse(istreamPtr inputStream);
    /// @endcond

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

    /**
     * Set the input stream for Parse.
     *
     * @param inputStream Input Stream 
     */
    void SetInputStream(istreamPtr inputStream);
   
  private:
    typedef std::map<std::string, StringVector::size_type> ColumnToIndexMap;
    typedef std::vector <StringVectorPtr> ErrorData;
    typedef std::vector <StringVectorPtr> RowData;
    typedef std::vector <StringVectorPtr> WarningData;

    void FixCompactArray(StringVector & compactVector, std::string context);
    /**
     * Parse the XML. An input stream must have already been
     * established. This method should only be called internally.
     *
     * @return Returns TRUE if a row was successfully parsed.
     */
    bool Parse();

    StringVectorPtr         mColumns;
    ColumnToIndexMap        mColumnToIndex;
    StringVectorPtr         mCurrentError;
    StringVectorPtr         mCurrentRow;
    StringVectorPtr         mCurrentWarning;
    std::string             mDelimiter;
    EncodingType            mEncoding;
    ErrorData               mErrors;
    ErrorData::iterator     mNextError;
    RowData::iterator       mNextRow;
    WarningData::iterator   mNextWarning;
    RowData                 mRows;
    istreamPtr              mParseInputStream;
    int                     mReplyCode;
    std::string             mReplyText;
    WarningData             mWarnings;
    ExpatXmlParserPtr       mXmlParser;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

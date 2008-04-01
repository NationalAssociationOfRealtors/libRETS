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

#include <string>
#include <vector>
#include "librets/std_forward.h"
#include "librets/ExpatXmlParser.h"
#include "librets/RetsObject.h"
#include "librets/RetsSession.h"

namespace librets {

/**
 * A search result set.
 */
class SearchResultSet : public virtual RetsObject
{
  public:
    /**
     * Default constructor.
     */
    SearchResultSet();

    /**
     * Virtual destructor.
     */
    virtual ~SearchResultSet();

    void Parse(istreamPtr inputStream);
    
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
     * @return The number of rows, or -1
     */
    int GetCount();

    /**
     * Returns the column names.
     *
     * @return The column names
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
     * Set the data encoding flag to allow for parsing of extended characters by Expat.
     * RETS is officially US-ASCII, but this will allow a work around for servers that
     * haven't properly sanitized their data.
     *
     * @param encoding Either RetsSession::RETS_XML_DEFAULT_ENCODING or 
     * RetsSession::RETS_XML_ISO_ENCODING.
     */
    void SetEncoding(RetsSession::EncodingType encoding);

    /**
     * Get the current value for the data encoding flag.
     *
     * @return string value of encoding flag.
     */
    RetsSession::EncodingType GetEncoding();
    
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
    
  private:
    typedef std::vector<StringVectorPtr> RowData;
    typedef std::map<std::string, StringVector::size_type> ColumnToIndexMap;
    void FixCompactArray(StringVector & compactVector, std::string context);
    /**
     * Parse the XML. An input stream must have already been
     * established. This method should only be called internally.
     *
     * @return Returns TRUE while more data can be fetched.
     */
    bool Parse();


    int mCount;
    StringVectorPtr mColumns;
    ColumnToIndexMap mColumnToIndex;
    RowData mRows;
    RowData::iterator mNextRow;
    StringVectorPtr mCurrentRow;
    RetsSession::EncodingType mEncoding;
    bool mMaxRows;
    int mReplyCode;
    std::string mReplyText;

    istreamPtr mParseInputStream;
    ExpatXmlParserPtr mXmlParser;
    std::string mDelimiter;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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

#ifndef LIBRETS_SEARCH_REQUEST_H
#define LIBRETS_SEARCH_REQUEST_H
/**
 * @file SearchRequest.h
 * Contains the SearchRequest class definition.
 */
#include <string>

#include "librets/RetsHttpRequest.h"

namespace librets {

/**
 * SearchRequest contains the API that is used to set up and
 * customer a RETS SEARCH transaciton.
 */
class SearchRequest : public RetsHttpRequest
{
  public:
    /**
     * Create a new search request.  Since the query type is depedent
     * on the RETS version, the RetsSession::CreateRetsRequest() is
     * the preferred way to create a RetsRequest. 
     *
     * @param searchType RETS resource name
     * @param searchClass RETS class name
     * @param query DMQL(2) query
     */
    SearchRequest(std::string searchType, std::string searchClass,
                  std::string query);
    
    /**
     * This requests the server default limit.  This should be the same as
     * NONE, but some servers do not adhere.
     */
    static const int LIMIT_DEFAULT = -1;
    
    /**
     * This requests the server to set the limit to NONE.
     */
    static const int LIMIT_NONE = 0;
    
    /**
     * Sets the limit on the number records returned.  By default, all
     * matching records are returned.  Setting the limit to
     * <code>0</code> means all matching records should be returned.
     *
     * @param limit The maximum number of records to return
     */
    void SetLimit(int limit);

    /**
     * This indicatates that no offset should be sent to the server.
     */
    static const int OFFSET_NONE = 0;

    /**
     * Sets the offset to be used for record retrieval.  Offsets are 1-based,
     * thus an offset of 1 means to start with the first record.
     */
    void SetOffset(int offset);

    /**
     * Sets the select string to choose which fields are returned.
     * Setting the select to an empty string returns all fields.  The
     * default select string is the empty string. The string should be
     * a comma delimited list of names from the metadata and are either
     * standard names or system names, depending on the value of the
     * standard names flag.
     *
     * @param select The select string
     */
    void SetSelect(std::string select);

    enum CountType
    {
        /**
         * The search result set will only contain records.  The record
         * count will always be 0.
         */
        NO_RECORD_COUNT,
        /**
         * The search result set will contain a valid cound and records.
         */
        RECORD_COUNT_AND_RESULTS,
        /**
         * The search result set will contain only a record count.  No records
         * will be in the result set.
         */
        RECORD_COUNT_ONLY
    };

    /**
     * Sets the count type.  By default, it is set to
     * RECORD_COUNT_AND_RESULTS.
     *
     * @param countType The count type
     */
    void SetCountType(CountType countType);
    
    /**
     * Sets the vocabular to system names or standard names.  If set to
     * <code>true</code> all fields will be standard names.  If set to
     * <code>false</code> all fields will be system names.  By default,
     * this is set to <code>false</code>.
     *
     * @param standardNames <code>true</code> for standard names.
     */
    void SetStandardNames(bool standardNames);
    
    enum QueryType
    {
        /** DMQL is required for RETS version 1.0. */
        DMQL,
        /** DMQL2 is required for all RETS 1.x versions, except 1.0. */
        DMQL2
    };
    
    /**
     * Sets the query type for this search.  This is highly dependent on
     * the RETS version of the server.  DMQL must be used for a 1.0 server.
     * DMQL2 must be used for all other 1.x versions.  You may use
     * RetsSession::CreateSearchRequest() to create a request with the
     * correct query type already set, based on the version of the currently
     * connected server.
     *
     * @param queryType The query type
     */
    void SetQueryType(QueryType queryType);

    enum FormatType
    {
        /**
         * COMPACT format returns values directly as they are contained
         * in the server.
         */
        COMPACT,

        /**
         * COMPACT_DECODED decodes some values into more human readable
         * values.  This is done on the server side, and the results are
         * server dependent.  However, this often implies that lookup fields
         * are decoded to their lookup values.
         */
        COMPACT_DECODED
    };

    /**
     * Sets the format of the data returned from the server.  The default
     * is COMPACT_DECODED.
     *
     * @param formatType The formate type
     */
    void SetFormatType(FormatType formatType);
    
  private:
    static const char * FORMAT_PARAMETER;
    static const char * STANDARD_NAMES_PARAMETER;
    static const char * QUERY_TYPE_PARAMETER;
    static const char * SEARCH_TYPE_PARAMETER;
    static const char * CLASS_PARAMETER;
    static const char * QUERY_PARAMETER;
    static const char * COUNT_PARAMETER;
    static const char * SELECT_PARAMETER;
    static const char * LIMIT_PARAMETER;
    static const char * OFFSET_PARAMETER;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

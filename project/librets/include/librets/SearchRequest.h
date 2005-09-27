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

#include <string>

#include "librets/RetsHttpRequest.h"

namespace librets {

class SearchRequestImpl;
/** Smart pointer to SearchRequestImpl. */
typedef boost::shared_ptr<SearchRequestImpl> SearchRequestImplPtr;

class SearchRequestConstants
{
  public:
    /**
     * This requests the server default limit.  This should be the same as
     * NONE, but some servers do not adhere.
     */
    static const int LIMIT_DEFAULT = -1;
    
    /**
     * This requests the server to set the limit to NONE.
     */
    static const int LIMIT_NONE = 0;

    enum CountType
    {
        NO_RECORD_COUNT,
        RECORD_COUNT_AND_RESULTS,
        RECORD_COUNT_ONLY
    };

    enum QueryType
    {
        DMQL,
        DMQL2
    };
};

/**
 * A search request.
 */
class SearchRequest : public RetsHttpRequest, public SearchRequestConstants
{
  public:
    /**
     * Create a new search request.
     *
     * @param searchType RETS resource name
     * @param searchClass RETS class name
     * @param query DMQL(2) query
     */
    SearchRequest(std::string searchType, std::string searchClass,
                  std::string query);

    /**
     * Sets the limit on the number records returned.  By default, all
     * matching records are returned.  Setting the limit to
     * <code>0</code> means all matching records should be returned.
     *
     * @param limit The maximum number of records to return
     */
    void SetLimit(int limit);

    /**
     * Sets the select string to choose which fields are returned.
     * Setting the select to an empty string returns all fields.  The
     * default select string is the empty string.
     *
     * @param select The select string
     */
    void SetSelect(std::string select);

    void SetCountType(CountType countType);
    
    void SetStandardNames(bool standardNames);
    
    void SetQueryType(QueryType queryType);
    
  private:
    SearchRequestImplPtr mImpl;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

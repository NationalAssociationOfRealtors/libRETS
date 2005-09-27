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

#ifndef LIBRETS_SEARCH_REQUEST_IMPL_H
#define LIBRETS_SEARCH_REQUEST_IMPL_H

#include <string>

#include "librets/RetsHttpRequestImpl.h"
#include "librets/SearchRequest.h"

namespace librets {
    
/**
 * A search request.
 */
class SearchRequestImpl : public RetsHttpRequestImpl, public SearchRequestConstants
{
  public:
    SearchRequestImpl(std::string searchType, std::string searchClass,
                      std::string query);

    void SetLimit(int limit);

    void SetSelect(std::string select);

    void SetCountType(CountType countType);
    
    void SetStandardNames(bool standardNames);
    
    void SetQueryType(QueryType queryType);
    
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
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

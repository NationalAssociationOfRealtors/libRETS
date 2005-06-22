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
#include "librets/SearchRequest.h"

using namespace librets;
using std::string;

SearchRequest::SearchRequest(string searchType, string searchClass,
                             string query)
{
    SetMethod(POST);
    SetQueryParameter("Format", "COMPACT-DECODED");
    SetQueryParameter("StandardNames", "1");
    SetQueryParameter("QueryType", "DMQL2");
    SetQueryParameter("SearchType", searchType);
    SetQueryParameter("Class", searchClass);
    SetQueryParameter("Query", query);
}

void SearchRequest::SetLimit(int limit)
{
    
}

void SearchRequest::SetSelect(string select)
{
    SetQueryParameter("Select", select);
}

void SearchRequest::SetCountType(CountType countType)
{
    switch (countType)
    {
        case RECORD_COUNT_AND_RESULTS:
            SetQueryParameter("Count", "1");
            break;

        case RECORD_COUNT_ONLY:
            SetQueryParameter("Count", "2");
            break;

        case NO_RECORD_COUNT:
        default:
            SetQueryParameter("Count", "0");
    }
}

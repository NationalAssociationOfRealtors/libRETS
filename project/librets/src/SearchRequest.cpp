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

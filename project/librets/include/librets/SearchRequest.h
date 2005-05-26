#ifndef LIBRETS_SEARCH_REQUEST_H
#define LIBRETS_SEARCH_REQUEST_H

#include <string>

#include "librets/RetsHttpRequest.h"

namespace librets {

/**
 * A search request.
 */
class SearchRequest : public RetsHttpRequest
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

    enum CountType
    {
        NO_RECORD_COUNT,
        RECORD_COUNT_AND_RESULTS,
        RECORD_COUNT_ONLY
    };

    void SetCountType(CountType countType);
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

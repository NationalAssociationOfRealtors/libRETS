#ifndef LIBRETS_SEARCH_REQUEST_H
#define LIBRETS_SEARCH_REQUEST_H

#include <string>

namespace librets {

/**
 * A search request.
 */
class SearchRequest
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
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

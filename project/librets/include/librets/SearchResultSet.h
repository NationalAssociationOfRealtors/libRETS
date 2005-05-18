#ifndef LIBRETS_SEARCH_RESULT_SET_H
#define LIBRETS_SEARCH_RESULT_SET_H

#include <string>
#include <vector>

namespace librets {

/**
 * A search result set.
 */
class SearchResultSet
{
  public:
    /**
     * Virtual destructor.
     */
    virtual ~SearchResultSet();

    /**
     * Returns true if there are more results.  This may block waiting
     * for data from the RETS server.
     */
    virtual bool HasNext() = 0;

    /**
     * Returns the number of rows in the result set, or -1 if not
     * known.  The number of rows is only available if it was
     * specified in the SearchRequest.
     *
     * @return The number of rows, or -1
     */
    virtual int GetCount() = 0;

    /**
     * Returns the column names.
     *
     * @return The column names
     */
    virtual std::vector<std::string> GetColumns() = 0;

    /**
     * Returns the value of a column as a string.
     *
     * @param columnIndex column index
     * @return string value of column
     */
    virtual std::string GetString(int columnIndex) = 0;

    /**
     * Returns the value of a column as a string.
     *
     * @param columnName column name
     * @return string value of column
     */
    virtual std::string GetString(std::string columnName) = 0;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

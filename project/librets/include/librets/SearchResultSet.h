#ifndef LIBRETS_SEARCH_RESULT_SET_H
#define LIBRETS_SEARCH_RESULT_SET_H

#include <string>
#include <vector>
#include "librets/std_forward.h"
#include "librets/RetsObject.h"

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
    StringVectorPtr GetColumns();

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

  private:
    typedef std::vector<StringVectorPtr> RowData;
    typedef std::map<std::string, StringVector::size_type> ColumnToIndexMap;

    int mCount;
    StringVectorPtr mColumns;
    ColumnToIndexMap mColumnToIndex;
    RowData mRows;
    RowData::iterator mNextRow;
    StringVectorPtr mCurrentRow;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

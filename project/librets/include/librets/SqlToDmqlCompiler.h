#ifndef LIBRETS_SQL_TO_DMQL_COMPILER_H
#define LIBRETS_SQL_TO_DMQL_COMPILER_H

#include <iosfwd>
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class SqlToDmqlCompiler : public RetsObject
{
  public:
    enum QueryType {DMQL_QUERY, GET_OBJECT_QUERY};
    
    QueryType sqlToDmql(std::string sql);
    
    QueryType sqlToDmql(std::istream & inputStream);
    
    DmqlQueryPtr GetDmqlQuery() const;
    
    GetObjectQueryPtr GetGetObjectQuery() const;
    
 private:
    DmqlQueryPtr mDmqlQuery;
    GetObjectQueryPtr mGetObjectQuery;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

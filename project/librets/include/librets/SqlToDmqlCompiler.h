#ifndef LIBRETS_SQL_TO_DMQL_COMPILER_H
#define LIBRETS_SQL_TO_DMQL_COMPILER_H

#include <iosfwd>
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class SqlToDmqlCompiler : public RetsObject
{
  public:
    DmqlQueryPtr sqlToDmql(std::string sql);
    
    DmqlQueryPtr sqlToDmql(std::istream & inputStream);
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

#ifndef LIBRETS_RETS_SQL_EXCEPTION_H
#define LIBRETS_RETS_SQL_EXCEPTION_H

#include "librets/RetsException.h"

namespace librets {

class RetsSqlException : public RetsException
{
  public:
    RetsSqlException(std::string message);

    virtual ~RetsSqlException() throw();
    
    virtual std::string GetName() const throw();
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

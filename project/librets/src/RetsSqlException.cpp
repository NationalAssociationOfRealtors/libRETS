#include "librets/RetsSqlException.h"

using namespace librets;
using std::string;

RetsSqlException::RetsSqlException(string message)
    : RetsException(message)
{
}

RetsSqlException::~RetsSqlException() throw()
{
}

string RetsSqlException::GetName() const throw()
{
    return "RetsSqlException";
}

#include <sstream>
#include "librets/DmqlCriterion.h"

using namespace librets;
using std::string;
using std::ostringstream;
using std::ostream;

DmqlCriterion::~DmqlCriterion()
{
}

std::string DmqlCriterion::ToDmqlString()
{
    ostringstream dmqlString;
    this->ToDmql(dmqlString);
    return dmqlString.str();
}

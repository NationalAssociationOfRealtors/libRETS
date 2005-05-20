#ifndef LIBRETS_STD_FORWARD_H
#define LIBRETS_STD_FORWARD_H

#include <iosfwd>
#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace librets {

typedef boost::shared_ptr<std::istream> istreamPtr;
typedef boost::shared_ptr<std::ostream> ostreamPtr;
typedef boost::shared_ptr<std::iostream> iostreamPtr;

typedef std::map<std::string, std::string> StringMap;

typedef std::vector<std::string> StringVector;
typedef boost::shared_ptr<StringVector> StringVectorPtr;

typedef std::vector<int> IntVector;
typedef boost::shared_ptr<IntVector> IntVectorPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

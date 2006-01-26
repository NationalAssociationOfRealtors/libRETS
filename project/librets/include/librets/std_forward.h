/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */

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

typedef std::auto_ptr<std::istream> istreamAPtr;
typedef std::auto_ptr<std::ostream> ostreamAPtr;
typedef std::auto_ptr<std::iostream> iostreamAPtr;

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

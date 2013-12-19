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
/** 
 * @file std_forward.h
 * (Internal) Contains a handful of declarations used system wide.
 */
/// @cond MAINTAINER

#include <iosfwd>
#include <string>
#include <map>
#include <vector>
#include <boost/shared_ptr.hpp>

namespace librets {
/** A smart pointer to a standard input stream. */
typedef boost::shared_ptr<std::istream> istreamPtr;
/** A smart pointer to a standard output stream. */
typedef boost::shared_ptr<std::ostream> ostreamPtr;
/** A smart pointer to a standard input/output stream. */
typedef boost::shared_ptr<std::iostream> iostreamPtr;
/** A smart pointer to an output stream wrapped around a file. */
typedef boost::shared_ptr<std::ofstream> ofstreamPtr;

/** An auto_ptr to a standard input stream. */
typedef std::auto_ptr<std::istream> istreamAPtr;
/** An auto_ptr to a standard output stream. */
typedef std::auto_ptr<std::ostream> ostreamAPtr;
/** An auto_ptr to a standard input/output stream. */
typedef std::auto_ptr<std::iostream> iostreamAPtr;

/** A key/value map using strings. */
typedef std::map<std::string, std::string> StringMap;
    
/** A key/value map with multiple keys. */
typedef std::multimap<std::string, std::string> StringMultiMap;

/** A vector of strings. */
typedef std::vector<std::string> StringVector;
/** A smart pointer to a vector of strings. */
typedef boost::shared_ptr<StringVector> StringVectorPtr;

/** A vector of integers. */
typedef std::vector<int> IntVector;
/** A smart pointer to a vector of integers. */
typedef boost::shared_ptr<IntVector> IntVectorPtr;

};

/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

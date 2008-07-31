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
#ifndef LIBRETS_DMQL_CRITERION_H
#define LIBRETS_DMQL_CRITERION_H
/** 
 * @file AndCriterion.h
 * (Internal) Contains the DmqlCriterion parser class.
 */
 /// @cond MAINTAINER

#include "librets/RetsObject.h"

namespace librets {

class DmqlCriterion : public virtual RetsObject
{
  public:
    virtual ~DmqlCriterion();

    virtual std::ostream & ToDmql(std::ostream & outputStream) const = 0;

    std::string ToDmqlString();
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

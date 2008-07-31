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
#ifndef LIBRETS_OR_CRITERION_H
#define LIBRETS_OR_CRITERION_H
/** 
 * @file OrCriterion.h
 * (Internal) Contains the OrCriterion parser class.
 */
 /// @cond MAINTAINER
#include <string>
#include "librets/sql_forward.h"
#include "librets/JunctionCriterion.h"

namespace librets {

class OrCriterion : public JunctionCriterion
{
  public:
    OrCriterion(DmqlCriterionPtr first, DmqlCriterionPtr second);

  protected:
    virtual std::string Operator() const;

    virtual std::string OperationName() const;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

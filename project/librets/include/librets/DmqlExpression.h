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
#ifndef LIBRETS_DMQL_EXPRESSION_H
#define LIBRETS_DMQL_EXPRESSION_H
/** 
 * @file DmqlExpression.h
 * (Internal) Contains the DmqlExpression parser class.
 */
 /// @cond MAINTAINER

#include "librets/sql_forward.h"

namespace librets {
namespace DmqlExpression {

DmqlCriterionPtr literal(std::string aString);

DmqlCriterionPtr dmqlString(std::string aString);

DmqlCriterionPtr literal(int number);

DmqlCriterionPtr eq(std::string field, DmqlCriterionPtr value);

DmqlCriterionPtr gt(std::string field, DmqlCriterionPtr value);

DmqlCriterionPtr lt(std::string field, DmqlCriterionPtr value);

DmqlCriterionPtr lookupOr(std::string field, DmqlCriterionPtr value);

DmqlCriterionPtr logicOr(DmqlCriterionPtr first, DmqlCriterionPtr second);

DmqlCriterionPtr logicAnd(DmqlCriterionPtr first, DmqlCriterionPtr second);

DmqlCriterionPtr logicNot(DmqlCriterionPtr criterion);

}; };
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

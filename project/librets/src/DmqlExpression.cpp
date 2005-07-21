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
#include <sstream>
#include "librets/DmqlExpression.h"
#include "librets/LiteralCriterion.h"
#include "librets/EqCriterion.h"
#include "librets/GtCriterion.h"
#include "librets/LtCriterion.h"
#include "librets/LookupOrCriterion.h"
#include "librets/OrCriterion.h"
#include "librets/AndCriterion.h"
#include "librets/NotCriterion.h"

using namespace librets;
using namespace librets::DmqlExpression;
using std::string;
using std::ostringstream;

#define NS librets::DmqlExpression

DmqlCriterionPtr NS::literal(string aString)
{
    LiteralCriterionPtr literal(new LiteralCriterion(aString));
    return literal;
}

DmqlCriterionPtr NS::literal(int number)
{
    ostringstream aString;
    aString << number;
    LiteralCriterionPtr literal(new LiteralCriterion(aString.str()));
    return literal;
}

DmqlCriterionPtr NS::eq(string field, DmqlCriterionPtr value)
{
    EqCriterionPtr eq(new EqCriterion(field, value));
    return eq;
}

DmqlCriterionPtr NS::gt(string field, DmqlCriterionPtr value)
{
    GtCriterionPtr gt(new GtCriterion(field, value));
    return gt;
}

DmqlCriterionPtr NS::lt(string field, DmqlCriterionPtr value)
{
    LtCriterionPtr lt(new LtCriterion(field, value));
    return lt;
}

DmqlCriterionPtr NS::lookupOr(string field, DmqlCriterionPtr value)
{
    LookupOrCriterionPtr lookupOr(new LookupOrCriterion(field, value));
    return lookupOr;
}

DmqlCriterionPtr NS::logicOr(DmqlCriterionPtr first, DmqlCriterionPtr second)
{
    OrCriterionPtr logicOr(new OrCriterion(first, second));
    return logicOr;
}

DmqlCriterionPtr NS::logicAnd(DmqlCriterionPtr first, DmqlCriterionPtr second)
{
    AndCriterionPtr logicAnd(new AndCriterion(first, second));
    return logicAnd;
}

DmqlCriterionPtr NS::logicNot(DmqlCriterionPtr criterion)
{
    NotCriterionPtr logicNot(new NotCriterion(criterion));
    return logicNot;
}

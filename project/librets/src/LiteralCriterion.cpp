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
#include <iostream>
#include "librets/LiteralCriterion.h"

using namespace librets;
using std::string;

LiteralCriterion::LiteralCriterion()
{
}

LiteralCriterion::LiteralCriterion(string aString)
    : mString(aString)
{
}

std::ostream & LiteralCriterion::ToDmql(std::ostream & outputStream) const
{
    return outputStream << mString;
}

std::ostream & LiteralCriterion::Print(std::ostream & outputStream) const
{
    return outputStream << "Literal Criterion [" << mString << "]";
}

bool LiteralCriterion::Equals(const RetsObject * object) const
{
    const LiteralCriterion * rhs =
        dynamic_cast<const LiteralCriterion *>(object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mString == rhs->mString);
    return equals;
}

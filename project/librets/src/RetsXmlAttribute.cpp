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
#include "librets/RetsXmlAttribute.h"

using namespace librets;
using namespace std;

RetsXmlAttribute::RetsXmlAttribute(string name, string value)
{
    mName = name;
    mValue = value;
}

string RetsXmlAttribute::GetName() const
{
    return mName;
}

string RetsXmlAttribute::GetValue() const
{
    return mValue;
}

ostream & RetsXmlAttribute::Print(ostream & outputStream) const
{
    return outputStream << "RetsXmlAttribute[" << mName << "="
                        << mValue << "]";
}

bool RetsXmlAttribute::Equals(const RetsObject * object) const
{
    const RetsXmlAttribute * rhs =
        dynamic_cast<const RetsXmlAttribute *>(object);
    if (rhs == 0)
    {
        return 0;
    }
    return ((mName == rhs->mName) && (mValue == rhs->mValue));
}

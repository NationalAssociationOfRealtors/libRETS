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
#include "librets/RetsXmlEndElementEvent.h"

using namespace librets;
using namespace std;

RetsXmlEndElementEvent::~RetsXmlEndElementEvent()
{
}

RetsXmlEvent::Type RetsXmlEndElementEvent::GetType() const
{
    return END_ELEMENT;
}

void RetsXmlEndElementEvent::SetName(string name)
{
    mName = name;
}

string RetsXmlEndElementEvent::GetName() const
{
    return mName;
}

ostream & RetsXmlEndElementEvent::Print(ostream & outputStream) const
{
    return outputStream << "XML end element: <" << mName << ">";
}

bool RetsXmlEndElementEvent::Equals(const RetsObject * object) const
{
    const RetsXmlEndElementEvent * rhs =
        dynamic_cast<const RetsXmlEndElementEvent *> (object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mName == rhs->GetName());
    return equals;
}

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
#include <string>
#include <iostream>
#include <sstream>
#include "librets/RetsObject.h"

#define CLASS RetsObject
#define NS librets

using namespace NS;
using std::string;
using std::ostream;
using std::ostringstream;


CLASS::~CLASS()
{
}

ostream & CLASS::Print(ostream & outputStream) const
{
    return outputStream << "RetsObject[" << (void *) this << "]";
}

string CLASS::ToString() const
{
    ostringstream outputStream;
    outputStream << *this;
    return outputStream.str();
}

bool CLASS::Equals(const RetsObject * rhs) const
{
    return (this == rhs);
}

ostream & NS::operator<<(ostream & outputStream,
                              const RetsObject & retsObject)
{
    return retsObject.Print(outputStream);
}

ostream & NS::operator<<(ostream & outputStream,
                              RetsObject * retsObject)
{
    if (retsObject == 0)
    {
        return outputStream << "null";
    }
    
    return retsObject->Print(outputStream);
}

bool NS::operator==(const RetsObject & lhs, const RetsObject & rhs)
{
    return lhs.Equals(&rhs);
}

bool NS::operator!=(const RetsObject & lhs, const RetsObject & rhs)
{
    return !lhs.Equals(&rhs);
}

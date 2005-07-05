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
#include "librets/RetsXmlTextEvent.h"

using namespace librets;
using namespace std;

RetsXmlTextEvent::RetsXmlTextEvent(int lineNumber, int columnNumber)
    : RetsXmlEvent(lineNumber, columnNumber)
{
}

RetsXmlTextEvent::~RetsXmlTextEvent()
{
}

RetsXmlEvent::Type RetsXmlTextEvent::GetType() const
{
    return TEXT;
}

void RetsXmlTextEvent::AppendText(string text)
{
    mText << text;
}

string RetsXmlTextEvent::GetText() const
{
    return mText.str();
}

ostream & RetsXmlTextEvent::Print(ostream & outputStream) const
{
    outputStream << "XML text event, <" << mText.str() << ">";
    return PrintLineAndColumn(outputStream);
}

bool RetsXmlTextEvent::Equals(const RetsObject * object) const
{
    const RetsXmlTextEvent * rhs =
        dynamic_cast<const RetsXmlTextEvent *> (object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mText.str() == (rhs->mText.str()));
    return equals;
}

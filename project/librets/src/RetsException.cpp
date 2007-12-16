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
#include <sstream>
#include "librets/RetsException.h"

using namespace librets;
using std::string;
using std::ostream;
using std::endl;
using std::ostringstream;

RetsException::RetsException(string message)
{
    mMessage = message;
    mExtendedMessage.clear();
}

RetsException::RetsException(string message, string extendedMessage)
{
    mMessage = message;
    mExtendedMessage = extendedMessage;
}

RetsException::~RetsException() throw()
{
}

string RetsException::GetName() const throw()
{
    return "RetsException";
}

void RetsException::SetContext(const RetsExceptionContext & context)
{
    mContext = context;
}

RetsExceptionContext RetsException::GetContext() const throw()
{
    return mContext;
}

string RetsException::GetMessage() const throw()
{
    return mMessage;
}

string RetsException::GetExtendedMessage() const throw()
{
    return mExtendedMessage;
}

const char * RetsException::what() const throw()
{
    return mMessage.c_str();
}

void RetsException::PrintContextMessage(ostream & out) const throw()
{
    out << GetName();
    if (mContext.IsValid())
    {
        out << " thrown at " << mContext.GetFileName() << ":"
            << mContext.GetLineNumber();
    }
}

std::string RetsException::GetContextMessage() const throw()
{
    ostringstream contextMessage;
    PrintContextMessage(contextMessage);
    return contextMessage.str();
}

void RetsException::PrintFullReport(ostream & out) const throw()
{
    PrintContextMessage(out);
    out << ": " << mMessage << endl;
    if (!mExtendedMessage.empty())
        out << mExtendedMessage << endl;
}

std::string RetsException::GetFullReport() const throw()
{
    ostringstream fullReport;
    PrintFullReport(fullReport);
    return fullReport.str();
}

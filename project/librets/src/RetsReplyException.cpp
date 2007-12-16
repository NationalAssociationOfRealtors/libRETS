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
#include "librets/RetsReplyException.h"

using namespace librets;
using std::string;

RetsReplyException::RetsReplyException(int replyCode, string meaning)
    : RetsException(meaning)
{
    mReplyCode = replyCode;
    mMeaning = meaning;
    mExtendedMeaning.clear();
}

RetsReplyException::RetsReplyException(int replyCode, string meaning, string extendedMeaning)
    : RetsException(meaning, extendedMeaning)
{
    mReplyCode = replyCode;
    mMeaning = meaning;
    mExtendedMeaning = extendedMeaning;
}

RetsReplyException::~RetsReplyException() throw()
{
}

int RetsReplyException::GetReplyCode() const throw()
{
    return mReplyCode;
}

string RetsReplyException::GetMeaning() const throw()
{
    return mMeaning;
}

string RetsReplyException::GetExtendedMeaning() const throw()
{
    return mExtendedMeaning;
}
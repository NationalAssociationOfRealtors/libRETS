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

#include "librets/StreamHttpLogger.h"
#include <ostream>
#include <time.h>

using namespace librets;
using std::ostream;
using std::endl;

StreamHttpLogger::StreamHttpLogger(ostream * out)
    : mLastType(INFORMATIONAL), mOut(out), mUseTimestamp(false)
{
}

void StreamHttpLogger::SetUseTimestamp(bool useTimestamp)
{
    mUseTimestamp = useTimestamp;
}

void StreamHttpLogger::logHttpData(Type type, std::string data)
{
    std::string timestr;
    if (mUseTimestamp)
    {
        time_t now;
        time(&now);
        timestr.append(" [").append(ctime(&now)).append("]");
    }
        
    if ((type == RECEIVED) && (mLastType != RECEIVED))
    {
        *mOut << endl << "<<< Received" << timestr << endl;
    }
    else if ((type == SENT) && (mLastType != SENT))
    {
        *mOut << endl << ">>> Sent" << timestr << endl;
    }
    else if (type == INFORMATIONAL)
    {
        *mOut << "* ";
    }
    *mOut << data;
    mOut->flush();
    mLastType = type;
}

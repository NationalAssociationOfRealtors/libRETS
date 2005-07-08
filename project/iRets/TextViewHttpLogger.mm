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

#import "TextViewHttpLogger.h"

using namespace librets;
using std::string;

TextViewHttpLogger::TextViewHttpLogger(NSTextView * textView)
{
    mTextView = textView;
    NSFont * monaco = [NSFont fontWithName: @"Monaco" size: 10];
    mReceivedAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:
        monaco, NSFontAttributeName,
        [NSColor redColor], NSForegroundColorAttributeName,
        0];
    mSentAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:
        monaco, NSFontAttributeName,
        [NSColor blueColor], NSForegroundColorAttributeName,
        0];
    mInformationalAttributes = [[NSDictionary alloc] initWithObjectsAndKeys:
        monaco, NSFontAttributeName,
        [NSColor blackColor], NSForegroundColorAttributeName,
        0];
    mShowInformational = YES;
}

TextViewHttpLogger::~TextViewHttpLogger()
{
    [mReceivedAttributes release];
    [mSentAttributes release];
    [mInformationalAttributes release];
}

void TextViewHttpLogger::setShowInformational(BOOL showInformational)
{
    mShowInformational = showInformational;
}

void TextViewHttpLogger::logHttpData(Type type, string data)
{
    NSString * nsData = [NSString stringWithCString: data.c_str()];
    NSDictionary * attributes;
    if (type == RECEIVED)
    {
        attributes = mReceivedAttributes;
    }
    else if (type == SENT)
    {
        attributes = mSentAttributes;
    }
    else // if (type == INFORMATIONAL)
    {
        if (!mShowInformational)
        {
            return;
        }
        attributes = mInformationalAttributes;
    }
    NSAttributedString * addendum =
        [[NSAttributedString alloc] initWithString: nsData
                                        attributes: attributes];
    NSTextStorage * textStorage = [mTextView textStorage];
    [textStorage appendAttributedString: addendum];
    NSRange endRange = NSMakeRange([textStorage length], 1);
    [mTextView scrollRangeToVisible: endRange];
}

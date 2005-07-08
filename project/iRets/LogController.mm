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

#import "LogController.h"

using namespace librets;

@implementation LogController

- (id) init
{
    self = [super init];
    mShowInformational = YES;
    return self;
}

- (void) awakeFromNib
{
    mLogger = new TextViewHttpLogger(mLogText);
    mLogger->setShowInformational(mShowInformational);
}

- (void) dealloc
{
    delete mLogger;
}

- (IBAction) showLogWindow: (id) sender
{
    if (![mLogPanel isVisible])
    {
        [self clearLog: sender];
    }
    [mLogPanel makeKeyAndOrderFront: nil];
}

- (IBAction) clearLog: (id) sender
{
    NSTextStorage * textStorage = [mLogText textStorage];
    NSRange fullRange = NSMakeRange(0, [textStorage length]);
    [textStorage replaceCharactersInRange: fullRange
                               withString: @""];
}

- (RetsHttpLogger *) logger
{
    return mLogger;
}

- (BOOL) showInformational
{
    return mShowInformational;
}

- (void) setShowInformational: (BOOL) showInformational
{
    mShowInformational = showInformational;
    mLogger->setShowInformational(mShowInformational);
}

@end

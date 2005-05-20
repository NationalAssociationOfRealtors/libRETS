//
//  MyArrayController.m
//  iRets
//
//  Created by Dave Dribin on 3/20/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "AccountsArrayController.h"


@implementation AccountsArrayController

- (id) init
{
    return self;
}

- (void) awakeFromNib
{
}

- (id) newObject
{
    NSMutableDictionary * object = [super newObject];
    [object setObject: @"Account Name" forKey: @"name"];
    return object;
}

- (void) prepareContent
{
}

@end

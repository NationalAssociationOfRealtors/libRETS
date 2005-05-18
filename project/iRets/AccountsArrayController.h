//
//  MyArrayController.h
//  iRets
//
//  Created by Dave Dribin on 3/20/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>


/**
 * The sole purpose of this class is to make sure the account name
 * gets prepopulated when creating new accounts.
 */
@interface AccountsArrayController : NSArrayController
{

}

- (id) init;

- (void) prepareContent;

@end

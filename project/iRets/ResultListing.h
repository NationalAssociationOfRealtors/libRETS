//
//  ResultListing.h
//  librets
//
//  Created by Dave Dribin on 4/6/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MyController.h"

@interface ResultListing : NSObject {
    NSMutableDictionary * mFields;
    NSMutableArray * mImages;
    MyController * mController;
}

-(id) initWithController: (MyController *) controller;

- (NSDictionary *) fields;
- (void) setField: (id) field forKey: (id) key;
- (BOOL) hasListingId;
- (NSArray *) images;

@end

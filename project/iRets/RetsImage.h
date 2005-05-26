//
//  RetsImage.h
//  librets
//
//  Created by Dave Dribin on 4/6/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import <Cocoa/Cocoa.h>
#import "MyController.h"
#include "librets.h"

@interface RetsImage : NSObject {
    NSString * mDescription;
    NSString * mObjectKey;
    int mObjectId;
    NSURL * mLocation;
    NSImage * mImage;
    BOOL mImageNeedsLoading;
    MyController * mController;
}

- (id) initWithObjectDescriptor: (librets::ObjectDescriptorPtr) objectDescriptor
                     controller: (MyController *) controller;

- (NSString *) description;
- (NSString *) objectKey;
- (int) objectId;
- (NSImage *) image;

@end

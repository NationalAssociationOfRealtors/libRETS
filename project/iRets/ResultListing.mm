//
//  ResultListing.mm
//  librets
//
//  Created by Dave Dribin on 4/6/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#include "librets.h"
#import "ResultListing.h"
#import "RetsImage.h"
#include <iostream>
using namespace std;

using namespace librets;

@interface ResultListing (Private)

- (void) fetchImages;
- (NSString *) listingId;

@end


@implementation ResultListing

- (id) initWithController: (MyController *) controller;
{
    mFields = [[NSMutableDictionary alloc] init];
    mImages = nil;
    mController = [controller retain];
    return self;
}

- (void) dealloc
{
    [mController release];
    if (mImages != nil)
        [mImages release];
    [mFields release];
}

- (NSDictionary *) fields
{
    return mFields;
}

- (void) setField: (id) field forKey: (id) key
{
    [mFields setObject: field forKey: key];
}

- (NSString *) listingId
{
    return [mFields objectForKey: @"ListingID"];
}

- (BOOL) hasListingId
{
    return ([self listingId] != nil);
}

- (NSArray *) images
{
    if (mImages == nil)
    {
        mImages = [[NSMutableArray alloc] init];
        [self fetchImages];
    }
    return mImages;
}

- (void) fetchImages
{
    try
    {
        GetObjectRequestPtr request(
            new GetObjectRequest("Property", "Photo"));
        const char * listingId = [[self listingId] cString];
        request->SetLocation(true);
        request->AddAllObjects(listingId);
        RetsSessionPtr session = [mController createRetsSession];
        GetObjectResponsePtr response = session->GetObject(request);
        ObjectDescriptorPtr objectDescriptor;
        while (objectDescriptor = response->NextObject())
        {
            RetsImage * image =
                [[RetsImage alloc] initWithObjectDescriptor: objectDescriptor
                                                 controller: mController];
            [mImages addObject: image];
        }
        session->Logout();
    }
    catch (const RetsException & e)
    {
        NSLog(@"Caught: %s", e.GetMessage().c_str());
    }
}


@end

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
    [super dealloc];
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
        GetObjectResponseAPtr response = session->GetObject(request.get());
        ObjectDescriptor * objectDescriptor;
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

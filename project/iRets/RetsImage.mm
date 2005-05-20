//
//  RetsImage.mm
//  librets
//
//  Created by Dave Dribin on 4/6/05.
//  Copyright 2005 __MyCompanyName__. All rights reserved.
//

#import "RetsImage.h"

using namespace librets;
using std::string;

@interface RetsImage (Private)

+ (NSData *) dataWithInputStream: (std::istream &) inputStream;

- (void) setImage: (NSImage *) image;
- (void) imageThreadEntry;
- (NSImage *) fetchImage: (NSURL *) url;
- (void) imageThreadExit: (NSImage *) image;

@end

@implementation RetsImage

- (id) initWithObjectDescriptor: (ObjectDescriptorPtr) objectDescriptor
                     controller: (MyController *) controller;
{
    mController = [controller retain];
    mDescription = nil;
    
    string description = objectDescriptor->GetDescription();
    if (!description.empty())
    {
        mDescription = [[NSString alloc] initWithCString: description.c_str()];
    }

    mObjectKey = [[NSString alloc] initWithCString:
        objectDescriptor->GetObjectKey().c_str()];
    mObjectId = objectDescriptor->GetObjectId();
    
    string locationUrl = objectDescriptor->GetLocationUrl();
    if (!locationUrl.empty())
    {
        NSString * locationUrlString =
            [[NSString alloc] initWithCString: locationUrl.c_str()];
        mLocation = [[NSURL alloc] initWithString: locationUrlString];
        mImage = nil;
        mImageNeedsLoading = YES;
    }
    else
    {
        mLocation = nil;
        istreamPtr inputStream = objectDescriptor->GetData();
        NSData * data = [RetsImage dataWithInputStream: *inputStream];
        mImage = [[NSImage alloc] initWithData: data];
        mImageNeedsLoading = NO;
    }

    return self;
}

#if 0
- (void) addObserver: (NSObject *) anObserver forKeyPath: (NSString *) keyPath
             options: (NSKeyValueObservingOptions) options
             context: (void *) context
{
    NSLog(@"Add observer: %@", anObserver);
    [super addObserver: anObserver forKeyPath: keyPath options: options
               context: context];
}

- (void) removeObserver: (NSObject *) anObserver
             forKeyPath: (NSString *) keyPath
{
    NSLog(@"Remove observer: %@", anObserver);
    [super removeObserver: anObserver forKeyPath: keyPath];
}
#endif

- (void) dealloc
{
    [mController release];
    if (mImage != nil)
        [mImage release];
    if (mLocation != nil)
        [mLocation release];
    if (mDescription != nil)
        [mDescription release];
    [mObjectKey release];
}

+ (NSData *) dataWithInputStream: (std::istream &) inputStream
{
    NSMutableData * data = [[[NSMutableData alloc] init] autorelease];
    
    while (!inputStream.eof())
    {
        char buffer[4096];
        int length;
        inputStream.read(buffer, sizeof(buffer));
        length = inputStream.gcount();
        [data appendBytes: buffer length: length];
    }
    return data;
}


- (NSString *) description
{
    return mDescription;
}

- (NSString *) objectKey
{
    return mObjectKey;
}

- (int) objectId
{
    return mObjectId;
}

- (NSImage *) image
{
    NSImage * image = mImage;
    if (mImageNeedsLoading)
    {
        [mController startImageLoading];
        mImageNeedsLoading = NO;
        [NSThread detachNewThreadSelector: @selector(imageThreadEntry:)
                                 toTarget: self withObject: nil];
    }
    return image;
}

- (void) setImage: (NSImage *) image;
{
    [mImage autorelease];
    mImage = [image retain];
}

+ (NSImage *) fetchImage: (NSURL *) url
{
//    [NSThread sleepUntilDate: [NSDate dateWithTimeIntervalSinceNow: 2]];
    return [[[NSImage alloc] initWithContentsOfURL: url] autorelease];
}

- (void) imageThreadEntry: (id) object
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    [[self retain] autorelease];
    
    NSImage * image = [RetsImage fetchImage: mLocation];
    [self performSelectorOnMainThread: @selector(imageThreadExit:)
                           withObject: image
                        waitUntilDone: YES];
    
    [pool release];
}

- (void) imageThreadExit: (NSImage *) image
{
    [self setImage: image];
    [mController endImageLoading];
}

@end

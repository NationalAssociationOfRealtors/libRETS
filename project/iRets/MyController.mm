#import "MyController.h"
#import "ResultListing.h"
#include "librets.h"

using namespace librets;

@interface MyController (Private)

- (void) terminateApplication: (NSNotification *) notification;
- (void) displayWarningForException: (RetsException &) exception;
- (void) displayWarning: (NSString *) message title: (NSString *) title;
- (void) displaySheet: (NSAlert *) alert;
- (NSString *) pathForDataFile: (NSString *) fileName;
- (void) saveQueries;
- (void) loadQueries;
- (void) addQuery: (NSString *) query;
- (void) setupTableColumns: (StringVectorPtr) resultColumns;
- (void) updateSqlView;
- (void) convertSqlToDmql;

- (void) executeQueryThreadEntry: (id) object;
- (NSMutableArray *) executeRetsSearch;
- (void) executeQueryThreadExit: (id) object;
- (void) setResultListings: (NSMutableArray *) resultListings;

- (void) fetchImagesThreadEntry: (ResultListing *) listing;
- (NSArray *) fetchImages: (ResultListing *) listing;
- (void) fetchImagesThreadExit: (NSArray *) images;

@end

@implementation MyController

- (id) init
{
    mAccounts = [[NSMutableArray alloc] init];
    mResultListings = [[NSMutableArray alloc] init];
    retsResource = @"Property";
    retsClass = @"ResidentialProperty";
    retsSelect = @"ListingID,ListPrice,City,ListDate";
    query = @"(ListPrice=0+)";
    mQueryType = @"DMQL";
    queryFont = [NSFont fontWithName: @"Monaco" size: 12.0];

    mPrefs = [[NSUserDefaults standardUserDefaults] retain];
    NSArray * accounts = [mPrefs arrayForKey: @"accounts"];
    NSEnumerator * accountEnumerator = [accounts objectEnumerator];
    NSDictionary * account;
    while (account = [accountEnumerator nextObject])
    {
        [mAccounts addObject: [account mutableCopy]];
    }
    
    mSavedQueries = [[NSMutableArray alloc] init];
    mSavedQueriesSet = [[NSMutableSet alloc] init];
    
    mCurrencyFields = [[NSSet alloc] initWithObjects:
        @"ListPrice", nil];
    
    mNumberFields = [[NSSet alloc] initWithObjects:
        @"ListPrice", @"Bedrooms", @"BathsTotal", nil];
    
    mDateFields = [[NSSet alloc] initWithObjects:
        @"ListDate", nil];
    mImages = nil;
    mImagesLoading = 0;
    mBusyMessage = nil;
    return self;
}

- (void) awakeFromNib
{
    [NSApp setDelegate: self];
    // For some reason, this setting does not take place in IB.
    [mSqlTextView setContinuousSpellCheckingEnabled: NO];
    [mDmqlTextView setContinuousSpellCheckingEnabled: NO];
    
    [mMainWindow center];
    [mTableView setTarget: self];
    [mTableView setDoubleAction: @selector(showPhotoWindow:)];
    [self loadQueries];
    [self willChangeValueForKey: @"resultListings"];
    [self didChangeValueForKey: @"resultListings"];
    [self updateSqlView];
}

- (void) dealloc
{
    NSLog(@"MyController: dealloc");
    [mBusyMessage release];
    [mImages release];
    [mDateFields release];
    [mNumberFields release];
    [mCurrencyFields release];
    [mSavedQueriesSet release];
    [mSavedQueries release];
    [mResultListings release];
    [mAccounts release];
    [mPrefs release];
}

- (NSMutableArray *) accounts
{
    return mAccounts;
}

- (void) setAccounts: (NSMutableArray *) accounts
{
    [mAccounts autorelease];
    mAccounts = [accounts retain];
}

- (NSMutableArray *) savedQueries
{
    return mSavedQueries;
}

- (void) addQuery: (NSString *) newQuery;
{
    if (![mSavedQueriesSet containsObject: newQuery])
    {
        [self willChangeValueForKey: @"savedQueries"];
        [mSavedQueries addObject: newQuery];
        [mSavedQueriesSet addObject: newQuery];
        [self didChangeValueForKey: @"savedQueries"];
    }
}

- (void) setSavedQueries: (NSMutableArray * ) savedQueries
{
    [mSavedQueries autorelease];
    mSavedQueries = [savedQueries retain];
}

- (NSString *) queryType
{
    return mQueryType;
}

- (void) setQueryType: (NSString *) queryType
{
    [self willChangeValueForKey: @"sqlQuery"];
    [mQueryType autorelease];
    mQueryType = [queryType retain];
    [self didChangeValueForKey: @"sqlQuery"];
    [self updateSqlView];
}

- (void) updateSqlView
{
    if ([self isSqlQuery])
    {
        [mSqlView setIsShown: YES];
        [mMainWindow makeFirstResponder: mSqlTextView];
        [mGenerateDmqlMenu setAction: @selector(convertSqlToDmql:)];
    }
    else
    {
        [mSqlView setIsShown: NO];
        [mMainWindow makeFirstResponder: mDmqlTextView];
        [mGenerateDmqlMenu setAction: nil];
    }
}

- (BOOL) isSqlQuery
{
    return [mQueryType isEqualToString: @"SQL"];
}

- (NSString *) pathForDataFile: (NSString *) fileName
{
    NSFileManager *fileManager = [NSFileManager defaultManager];
    
    NSString *folder = @"~/Library/Application Support/iRets/";
    folder = [folder stringByExpandingTildeInPath];
    
    if ([fileManager fileExistsAtPath: folder] == NO)
    {
        [fileManager createDirectoryAtPath: folder attributes: nil];
    }
    
    return [folder stringByAppendingPathComponent: fileName];    
}

- (IBAction) clearSavedQueries: (id) sender
{
    [self willChangeValueForKey: @"savedQueries"];
    [mSavedQueries removeAllObjects];
    [mSavedQueriesSet removeAllObjects];
    [self didChangeValueForKey: @"savedQueries"];
}

- (void) loadQueries
{
    NSString * path = [self pathForDataFile: @"saved_queries.plist"];
    NSData * plistData = [NSData dataWithContentsOfFile: path];
    NSString *error;
    NSPropertyListFormat format;
    NSMutableArray * plist = [NSPropertyListSerialization
        propertyListFromData: plistData
            mutabilityOption: NSPropertyListMutableContainers
                      format: &format
            errorDescription: &error];
    if(!plist)
    {
        NSLog(error);
        [error release];
    }
    [self willChangeValueForKey: @"savedQueries"];
    [self setSavedQueries: plist];
    [mSavedQueriesSet removeAllObjects];
    [mSavedQueriesSet addObjectsFromArray: mSavedQueries];
    [self didChangeValueForKey: @"savedQueries"];
}

- (void) saveQueries
{
    NSString * path = [self pathForDataFile: @"saved_queries.plist"];
    NSString * error;
    NSData * xmlData =
        [NSPropertyListSerialization
            dataFromPropertyList: mSavedQueries
                          format: NSPropertyListXMLFormat_v1_0
                errorDescription: &error];
    
    if(xmlData)
    {
        [xmlData writeToFile:path atomically:YES];
    }
    else
    {
        NSLog(error);
        [error release];
    }
}

- (void) applicationWillTerminate: (NSNotification *) notification
{
    [mPrefs setObject: mAccounts forKey: @"accounts"];
    [self saveQueries];
}

- (BOOL) applicationShouldTerminateAfterLastWindowClosed:
    (NSApplication *) theApplication
{
    return YES;
}

- (void) addAccount: (id) sender
{
    [mAccountsController add: sender];
    [mPreferencesPanel makeFirstResponder: mAccountNameField];
}


NSString * GetString(SearchResultSetPtr results, std::string column)
{
    std::string value = results->GetString(column);
    return [NSString stringWithCString: value.c_str()];
}

NSNumber * GetNumber(SearchResultSetPtr results, std::string column)
{
    NSString * string = GetString(results, column);
    NSNumber * number = [NSNumber numberWithInt: [string intValue]];
    return number;
}

NSDate * GetDate(SearchResultSetPtr results, std::string column)
{
    NSString * string = GetString(results, column);
    NSDate * date = [NSDate dateWithNaturalLanguageString: string];
    return date;
}

std::string cppString(NSString * aString)
{
    if (aString == nil)
    {
        return "";
    }
    else
    {
        return std::string([aString cString]);
    }
}

NSString * toNSString(const std::string aString)
{
    return [NSString stringWithCString: aString.c_str()];
}

- (RetsSessionPtr) createRetsSession
{
    int selectedItem = [mSelectedAccount indexOfSelectedItem];
    NSDictionary * account = [mAccounts objectAtIndex: selectedItem];
    NSString * loginUrl = [account objectForKey: @"url"];
    NSString * userName = [account objectForKey: @"userName"];
    NSString * password = [account objectForKey: @"password"];

    RetsSessionPtr session(new RetsSession([loginUrl cString]));
    bool loggedIn = session->Login([userName cString], [password cString]);
    if (!loggedIn)
    {
        throw RetsException("Unable to log in");
    }
    return session;
}

- (void) executeQuery: (id) sender
{
    if ([self isSqlQuery])
    {
        [self convertSqlToDmql: sender];
    }
    
    if (query == nil)
    {
        return;
    }

#if 0
    [self addQuery: query];
    
    [self willChangeValueForKey: @"resultListings"];
    [mResultListings removeAllObjects];
    [self didChangeValueForKey: @"resultListings"];
#endif

    [self setBusyMessage: @"Executing Query"];
    [NSThread detachNewThreadSelector: @selector(executeQueryThreadEntry:)
                             toTarget: self withObject: nil];
}

- (void) executeQueryThreadEntry: (id) object
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];

    NSMutableArray * resultListings = [self executeRetsSearch];
    [self performSelectorOnMainThread: @selector(executeQueryThreadExit:)
                           withObject: resultListings
                        waitUntilDone: YES];
    
    [pool release];
}

- (NSMutableArray *) executeRetsSearch
{
    NSMutableArray * resultListings =
        [[[NSMutableArray alloc] init] autorelease];
    try
    {
        RetsSessionPtr session = [self createRetsSession];
        SearchRequestPtr searchRequest(
            new SearchRequest(cppString(retsResource), cppString(retsClass),
                              cppString(query)));
        searchRequest->SetSelect(cppString(retsSelect));
        searchRequest->SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
        SearchResultSetPtr resultSet = session->Search(searchRequest);
        StringVectorPtr columns = resultSet->GetColumns();
        [self setupTableColumns: columns];
        while (resultSet->HasNext())
        {
            StringVector::iterator i;
            ResultListing * listing =
                [[ResultListing alloc] initWithController: self];
            for (i = columns->begin(); i != columns->end(); i++)
            {
                NSString * columnName = toNSString(*i);
                if ([mNumberFields containsObject: columnName])
                {
                    [listing setField: GetNumber(resultSet, *i)
                               forKey: columnName];
                }
                else if ([mDateFields containsObject: columnName])
                {
                    [listing setField: GetDate(resultSet, *i)
                               forKey: columnName];
                }
                else
                {
                    [listing setField: GetString(resultSet, *i)
                               forKey: columnName];
                }
            }
//            [self willChangeValueForKey: @"resultListings"];
            [resultListings addObject: listing];
//            [self didChangeValueForKey: @"resultListings"];
        }
        session->Logout();
    }
    catch (RetsException & e)
    {
        [self displayWarningForException: e];
    }
    return resultListings;
}

- (void) executeQueryThreadExit: (NSMutableArray *) resultListings
{
    [self setResultListings: resultListings];
    [self setBusyMessage: nil];
}

- (NSArray *) resultListings
{
    return mResultListings;
}

- (void) setResultListings: (NSMutableArray *) resultListings;
{
    [mResultListings autorelease];
    mResultListings = [resultListings retain];
}

- (void) setupTableColumns: (StringVectorPtr) resultColumns
{
    NSArray * tableColumns = [mTableView tableColumns];
    NSEnumerator * columnEnumerator = [tableColumns objectEnumerator];
    NSTableColumn * column;
    while (column = [columnEnumerator nextObject])
    {
        [mTableView removeTableColumn: column];
    }
    
    StringVector::iterator i;
    for (i = resultColumns->begin(); i != resultColumns->end(); i++)
    {
        NSString * identifier = [NSString stringWithCString: i->c_str()];
        column = [[NSTableColumn alloc] initWithIdentifier: identifier];
        [column setEditable: NO];
        [[column headerCell] setTitle: identifier];
        NSTextFieldCell * cell = [[NSTextFieldCell alloc] init];
        if ([mCurrencyFields containsObject: identifier])
        {
            [cell setFormatter: mCurrencyFormatter];
        }
        if ([mNumberFields containsObject: identifier])
        {
            [cell setAlignment: NSRightTextAlignment];
        }
        if ([mDateFields containsObject: identifier])
        {
            [cell setFormatter: mDateFormatter];
        }
        [column setDataCell: cell];
        [mTableView addTableColumn: column];
        
        NSMutableString * keyPath = [NSMutableString stringWithString: 
            @"arrangedObjects.fields."];
        [keyPath appendString: identifier];
        [column bind: @"value" toObject: mResultsController 
         withKeyPath: keyPath options: nil];
    }
}

- (void) convertSqlToDmql: (id) sender
{
    try
    {
        if (sqlQueryString == nil)
        {
            [self setValue: nil forKey: @"query"];
            return;
        }

        SqlToDmqlCompiler compiler;
        std::string cppQuery = [sqlQueryString cString];
        SqlToDmqlCompiler::QueryType queryType = compiler.sqlToDmql(cppQuery);
        if (queryType != SqlToDmqlCompiler::DMQL_QUERY)
        {
            [self displayWarning: @"The SQL is not a search query"
                           title: @"Not a Search Query"];
            [self setValue: nil forKey: @"query"];
            return;
        }
        DmqlQueryPtr dmqlQuery = compiler.GetDmqlQuery();
        std::string select = librets::util::join(*dmqlQuery->GetFields(), ",");
        [self setValue: toNSString(dmqlQuery->GetResource()) forKey: @"retsResource"];
        [self setValue: toNSString(dmqlQuery->GetClass()) forKey: @"retsClass"];
        [self setValue: toNSString(select) forKey: @"retsSelect"];
        [self setValue: toNSString(dmqlQuery->GetCriterion()->ToDmqlString())
                forKey: @"query"];
    }
    catch (RetsException & e)
    {
        [self displayWarningForException: e];
    }
}

- (void) displayWarningForException: (RetsException &) exception
{
    std::string cppReport = exception.GetFullReport();
    const char * cReport = cppReport.c_str();
    NSLog(@"Caught exception: %s", cReport);
    NSString * message = [NSString stringWithCString: exception.what()];
    NSString * title = @"An error occured comminicating with the RETS Server";
    [self displayWarning: message title: title];
}

- (void) displayWarning: (NSString *) message title: (NSString *) title
{
    NSAlert *alert = [[NSAlert alloc] init];
    [alert addButtonWithTitle:@"OK"];
    [alert setMessageText: title];
    [alert setInformativeText: message];
    [alert setAlertStyle: NSWarningAlertStyle];
    [self performSelectorOnMainThread: @selector(displaySheet:)
                           withObject: alert
                        waitUntilDone: NO];
}

- (void) displaySheet: (NSAlert *) alert
{
    [alert autorelease];
    [alert beginSheetModalForWindow: mMainWindow
                      modalDelegate: nil
                     didEndSelector: nil
                        contextInfo: nil];
}

- (NSArray *) images
{
    return mImages;
}

- (void) setImages: (NSArray *) images
{
    // These notifications don't seem necessary, but the description
    // doesn't get filled in correctly, otherwise
    [self willChangeValueForKey: @"images"];
    [mImages autorelease];
    mImages = [images retain];
    [self didChangeValueForKey: @"images"];
}

- (void) startImageLoading
{
    [self willChangeValueForKey: @"imageLoading"];
    mImagesLoading++;
    [self didChangeValueForKey: @"imageLoading"];
}

- (void) endImageLoading
{
    [self willChangeValueForKey: @"images"];
    [self didChangeValueForKey: @"images"];
    
    [self willChangeValueForKey: @"imageLoading"];
    mImagesLoading--;
    [self didChangeValueForKey: @"imageLoading"];
}

- (BOOL) isImageLoading
{
    return (mImagesLoading != 0);
}

- (IBAction) showPhotoWindow: (id) sender
{
    if ([self isBusy])
    {
        return;
    }

    // Use clickedRow, since double clicking on a header will also trigger
    // this action, but will return -1;
    int rowIndex = [mTableView clickedRow];
    if (rowIndex < 0)
    {
        return;
    }
    // Use the controller's array since the items may be sorted different
    // than our mResultListings array
    NSArray * arrangedObjects = [mResultsController arrangedObjects];
    ResultListing * listing = [arrangedObjects objectAtIndex: rowIndex];
    if (![listing hasListingId])
    {
        [self displayWarning: @"You must include the ListingID to view photos"
                       title: @"Cannot View Photo"];
        return;
    }
    
    [self setBusyMessage: @"Loading Photos"];
    [NSThread detachNewThreadSelector: @selector(fetchImagesThreadEntry:)
                             toTarget: self withObject: listing];
}

- (void) fetchImagesThreadEntry: (ResultListing *) listing
{
    NSAutoreleasePool * pool = [[NSAutoreleasePool alloc] init];
    
    NSArray * images = [self fetchImages: listing];
    [self performSelectorOnMainThread: @selector(fetchImagesThreadExit:)
                           withObject: images
                        waitUntilDone: YES];
    
    [pool release];
}

- (NSArray *) fetchImages: (ResultListing *) listing;
{
    NSArray * images = [listing images];
    if ([images count] == 0)
    {
        return nil;
    }
    return images;
}

- (void) fetchImagesThreadExit: (NSArray *) images
{
    [self setBusyMessage: nil];
    if (images != nil)
    {
        [self setImages: images];
        [mPhotoPanel makeKeyAndOrderFront: nil];
    }
    else
    {
        [self displayWarning: @"No photos where found for this listing."
                       title: @"No Photos For Listing"];
    }
}

- (NSString *) busyMessage
{
    return mBusyMessage;
}

- (void) setBusyMessage: (NSString *) busyMessage
{
    [self willChangeValueForKey: @"busy"];
    [mBusyMessage autorelease];
    mBusyMessage = [busyMessage retain];
    [self didChangeValueForKey: @"busy"];
}

- (BOOL) isBusy
{
    return (mBusyMessage != nil);
}

@end

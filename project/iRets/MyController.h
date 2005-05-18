/* MyController */

#import <Cocoa/Cocoa.h>
#import <DisclosableView/DisclosableView.h>
#include "librets.h"

@interface MyController : NSObject
{
    NSMutableArray * mAccounts;
    NSUserDefaults * mPrefs;

    IBOutlet NSPanel * mPreferencesPanel;
    IBOutlet NSTextField * mAccountNameField;
    IBOutlet NSArrayController * mAccountsController;

    IBOutlet NSWindow * mMainWindow;
    IBOutlet NSArrayController * mResultsController;
    IBOutlet NSPopUpButton * mSelectedAccount;
    IBOutlet NSTableView * mTableView;
    IBOutlet NSFormatter * mCurrencyFormatter;
    IBOutlet NSFormatter * mDateFormatter;
    IBOutlet SNDisclosableView * mSqlView;
    IBOutlet NSTextView * mSqlTextView;
    IBOutlet NSTextView * mDmqlTextView;
    IBOutlet NSMenuItem * mGenerateDmqlMenu;
    IBOutlet NSPanel * mPhotoPanel;
        
    NSString * retsResource;
    NSString * retsClass;
    NSString * retsSelect;
    NSString * query;
    NSString * sqlQueryString;
    NSMutableArray * mResultListings;
    NSMutableArray * mSavedQueries;
    NSMutableSet * mSavedQueriesSet;
    NSSet * mCurrencyFields;
    NSSet * mNumberFields;
    NSSet * mDateFields;
    NSString * mQueryType;
    NSFont * queryFont;
    NSArray * mImages;
    int mImagesLoading;
    NSString * mBusyMessage;
}

- (id) init;
- (void) awakeFromNib;
- (void) dealloc;

- (NSMutableArray *) accounts;
- (void) setAccounts: (NSMutableArray *) acounts;

- (NSMutableArray *) savedQueries;
- (void) setSavedQueries: (NSMutableArray *) savedQueries;

- (NSString *) queryType;
- (void) setQueryType: (NSString *) queryType;
- (BOOL) isSqlQuery;

- (NSArray *) images;
- (void) setImages: (NSArray *) images;

- (void) startImageLoading;
- (void) endImageLoading;
- (BOOL) isImageLoading;

- (NSString *) busyMessage;
- (void) setBusyMessage: (NSString *) busyMessage;
- (BOOL) isBusy;

- (NSArray *) resultListings;

- (IBAction) addAccount: (id) sender;

- (IBAction) executeQuery: (id) sender;
- (IBAction) convertSqlToDmql: (id) sender;

- (IBAction) clearSavedQueries: (id) sender;

- (IBAction) showPhotoWindow: (id) sender;

- (librets::RetsSessionPtr) createRetsSession;

@end

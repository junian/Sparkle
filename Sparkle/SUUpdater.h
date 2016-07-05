//
//  SUUpdater.h
//  Sparkle
//
//  Created by Andy Matuschak on 1/4/06.
//  Copyright 2006 Andy Matuschak. All rights reserved.
//

#ifndef SUUPDATER_H
#define SUUPDATER_H

#if __has_feature(modules)
@import Foundation;
#else
#import <Foundation/Foundation.h>
#endif
#import "SUExport.h"
#import "SUUserDriver.h"

@class SUAppcastItem, SUAppcast;

@protocol SUUpdaterDelegate;

/*!
    The main API in Sparkle for controlling the update mechanism.

    This class is used to configure the update paramters as well as manually
    and automatically schedule and control checks for updates.
 */
SU_EXPORT @interface SUUpdater : NSObject

/*!
 * Initializes a new SUUpdater instance
 *
 * This does not start the updater. To start it, see -[SUUpdater startUpdater:]
 *
 * Note that this is a normal initializer and doesn't implement the singleton pattern (i.e, instances aren't cached, so no surprises)
 * Hence you shouldn't create multiple live instances that target the same bundle.
 * However, this also means that updater instances can be deallocated, and that they will be torn down properly
 *
 * Related: See SUStandardUpdaterController which wraps a SUUpdater instance and is suitable for instantiating in nib files
 *
 * @param hostBundle The bundle that should be targetted for updating. This must not be nil.
 * @param userDriver The user driver that Sparkle uses for user update interaction
 * @param delegate The delegate for SUUpdater. This may be nil.
 *
 * This must be called on the main thread.
 */
- (instancetype)initWithHostBundle:(NSBundle *)hostBundle userDriver:(id <SUUserDriver>)userDriver delegate:(id <SUUpdaterDelegate>)delegate;

/*!
 * Starts the updater.
 *
 * This method checks if Sparkle is configured properly. A valid feed URL should be set before this method is invoked.
 * Other properties of this SUUpdater instance can be set before this method is invoked as well, such as automatic update checks.
 * If the configuration is valid, this method may bring up a permission prompt (if needed) for checking if the user wants automatic update checking.
 * This method then starts the regular update cycle if automatic update checks are enabled.
 *
 * @param error The error that is populated if this method fails. Pass NULL if not interested in the error information.
 * @return YES if the updater started otherwise NO with a populated error
 */
- (BOOL)startUpdater:(NSError * __autoreleasing *)error;

@property (weak, readonly) id<SUUpdaterDelegate> delegate;
@property (nonatomic, readonly) id<SUUserDriver> userDriver;

@property (readonly, strong) NSBundle *hostBundle;
@property (strong, readonly) NSBundle *sparkleBundle;

@property (nonatomic) BOOL automaticallyChecksForUpdates;

@property (nonatomic) NSTimeInterval updateCheckInterval;

/*!
 * The URL of the appcast used to download update information.
 *
 * This property must be called on the main thread.
 */
@property (copy) NSURL *feedURL;

@property (nonatomic, copy) NSString *userAgentString;

@property (copy) NSDictionary *httpHeaders;

@property (nonatomic) BOOL sendsSystemProfile;

@property (nonatomic) BOOL automaticallyDownloadsUpdates;

/*!
    Checks for updates, and displays progress while doing so
 
    This is meant for users initiating an update check
 */
- (void)checkForUpdates;

/*!
    Checks for updates, but does not display any UI unless an update is found.

    This is meant for programmatically initating a check for updates. That is,
    it will display no UI unless it actually finds an update, in which case it
    proceeds as usual.

    If the fully automated updating is turned on, however, this will invoke that
    behavior, and if an update is found, it will be downloaded and prepped for
    installation.
 */
- (void)checkForUpdatesInBackground;

/*!
    Returns the date of last update check.

    \returns \c nil if no check has been performed.
 */
@property (readonly, copy) NSDate *lastUpdateCheckDate;

/*!
    Begins a "probing" check for updates which will not actually offer to
    update to that version.

    However, the delegate methods
    SUUpdaterDelegate::updater:didFindValidUpdate: and
    SUUpdaterDelegate::updaterDidNotFindUpdate: will be called,
    so you can use that information in your UI.
 */
- (void)checkForUpdateInformation;

/*!
    Appropriately schedules or cancels the update checking timer according to
    the preferences for time interval and automatic checks.

    This call does not change the date of the next check,
    but only the internal timer.
 */
- (void)resetUpdateCycle;

@end

#endif

//
//  CoreDataEnvir.h
//  CoreDataLab
//
//	CoreData enviroment light wrapper.
//	Support CoreData operating methods.
//
//	Create record item.
//	Support concurrency operating.
//
//  Created by NicholasXu on 11-5-25.
//
//  mailto:dehengxu@outlook.com
//
//  Copyright 2011 NicholasXu. All rights reserved.
//


#import <UIKit/UIKit.h>
#import <CoreData/CoreData.h>

#define CORE_DATA_ENVIR_SHOW_LOG        0
/**
 Triggle to enable persistance shared.
 1: Multi context shared same persistence file.
 0: Every context has own persistence file.
 */
#define CORE_DATA_SHARE_PERSISTENCE     0

#pragma mark - ------------------------------ CoreDataEnvirObserver (Not be used temporarily) ---------------------------

@protocol CoreDataEnvirObserver

@optional
- (void)didFetchingFinished:(NSArray *) aItems;
- (void)didUpdateContext:(id)sender;
- (void)didDeleteObjects:(id)sender;
- (void)didInsertObjects:(id)sender;
- (void)didUpdateObjects:(id)sender;

@end

#pragma mark - ------------------------------ CoreDataEnvirement -----------------------

@interface CoreDataEnvir : NSObject <NSFetchedResultsControllerDelegate> {
	NSManagedObjectModel * model;
	NSManagedObjectContext * context;
	NSFetchedResultsController * fetchedResultsCtrl;
    NSRecursiveLock *recursiveLock;
}

/**
 A model object.
 */
@property (nonatomic, retain) NSManagedObjectModel	*model;

/**
 A context object.
 */
@property (nonatomic, readonly) NSManagedObjectContext *context;

#if !CORE_DATA_SHARE_PERSISTENCE
/**
 A persistance coordinator object.
 */
@property (nonatomic, retain) NSPersistentStoreCoordinator *storeCoordinator;
#endif

/**
 A NSFetchedResultsController object, not be used by now.
 */
@property (nonatomic, retain) NSFetchedResultsController * fetchedResultsCtrl;

/**
 Model file name. It normally be name.momd
 */
@property (nonatomic, copy, setter = registModelFileName:, getter = modelFileName) NSString *modelFileName;

/**
 Data base file name. It can be whatever you want.
 */
@property (nonatomic, copy, setter = registDatabaseFileName:, getter = databaseFileName) NSString *databaseFileName;

/**
 Data file root path.
 */
@property (nonatomic, copy, setter = registDataFileRootPath:, getter = dataRootPath) NSString *dataRootPath;

/**
 Regist the specified model file name.
 
 @prarm name    xcdatamodeld file name.(Except file extension.)
 */
+ (void)registDefaultModelFileName:(NSString *)name;

/**
 Regist the specified data file name.
 
 @param name    Data file name.(Except path.)
 */
+ (void)registDefaultDataFileName:(NSString *)name;

/**
 Regist the specified path as data file root.
 
 @param path    Data file root path.
 */
+ (void)registDefaultDataFileRootPath:(NSString *)path;

/**
 Get model file name.(Name likes: xxxx.mmod in sandbox.)
 */
+ (NSString *)defaultModelFileName;

/**
 Get data base file name.
 */
+ (NSString *)defaultDatabaseFileName;

/**
 Creating instance conditionally.
 If current thread is main thread return single main instance,else return an temporary new instance.
 */
+ (CoreDataEnvir *)instance;

/**
 Create an CoreDataEnvir instance by specified db file name and momd file name.
 The momd file name is a middle file generated by XCode handle xcdatamodeld file.
 
 @param databaseFileName    A specified db file name.
 @param modelFileName       A specified momd file name.
 */
+ (CoreDataEnvir *)createInstanceWithDatabaseFileName:(NSString *)databaseFileName modelFileName:(NSString *)modelFileName;

/**
 Only returen a single instance runs on main thread.
 */
+ (CoreDataEnvir *)mainInstance;

/**
 Creating a new instance by default db, momd file name.
 */
+ (CoreDataEnvir *)createInstance;

/**
 Release the main instance.
 */
//+ (void)deleteInstance;

/**
 Return data root path
 */
+ (NSString *)dataRootPath;


/**
 Init instance with specified db , model file name.
 
 @param databaseFileName    db file name.
 @param modelFileName       Model mapping file name.
 @return CoreDataEnvir instance.
 */
- (id)initWithDatabaseFileName:(NSString *)databaseFileName modelFileName:(NSString *)modelFileName;

/**
 Save
 */
- (BOOL)saveDataBase;

@end

#pragma mark -  NSObject (Debug_Ext)

/**
 NSObject (Debug_Ext)
 */
@interface NSObject (Debug_Ext)

/**
 Get current dispatch queue label string.
 */
- (NSString *)currentDispatchQueueLabel;

@end

#pragma mark - NSmanagedObject convinent methods.

@interface NSManagedObject (CONVENIENT)

#pragma mark - Operation on main thread.
/**
 Creating managed object on main thread.
 */
+ (id)insertItem;

/**
 Creating managed object in main context by filling 'block'
 */
+ (id)insertItemWithBlock:(void(^)(id item))settingBlock;

/**
 Just fetching record items by the predicate in main context.
 */
+ (NSArray *)items;

/**
 Fetch record items in main context by predicate.
 */
+ (NSArray *)itemsWithPredicate:(NSPredicate *)predicate;

/**
 Fetch record items in main context by formated string.
 */
+ (NSArray *)itemsWithFormat:(NSString *)fmt,...;

/**
 Fetching last record item.
 */
+ (id)lastItem;

/**
 Fetch record item by predicate in main context.
 */
+ (id)lastItemWithPredicate:(NSPredicate *)predicate;

/**
 Fetch record item by formated string in main context.
 */
+ (id)lastItemWithFormat:(NSString *)fmt,...;

/**
 Remove item.
 */
- (void)remove;

/**
 Save db on main thread.
 */
- (BOOL)save;


#pragma mark - Operation on other sperate thread.

/**
 Creating managed object on background thread.
 */
+ (id)insertItemInContext:(CoreDataEnvir *)cde;

/**
 Createing managed object in specified context with filling 'block'
 */
+ (id)insertItemInContext:(CoreDataEnvir *)cde fillData:(void (^)(id item))settingBlock;

/**
 Fetching record items in specified context.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde;

/**
 Fetch record items by predicate.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde usingPredicate:(NSPredicate *)predicate;

/**
 Fetch record items by format string.
 */
+ (NSArray *)itemsInContext:(CoreDataEnvir *)cde withFormat:(NSString *)fmt,...;

/**
 Fetch item in specified context.
 */
+ (id)lastItemInContext:(CoreDataEnvir *)cde;

/**
 Fetch item in specified context through predicate.
 */
+ (id)lastItemInContext:(CoreDataEnvir *)cde usingPredicate:(NSPredicate *)predicate;

/**
 Fetch item in specified context through format string.
 */
+ (id)lastItemInContext:(CoreDataEnvir *)cde withFormat:(NSString *)fmt,...;

/**
 Update NSManagedObject if faulted.
 */
- (id)update;

/**
 Update NSManagedObject in specified context if faulted.
 
 @param cde     CoreDataEnvir object.
 */
- (id)updateInContext:(CoreDataEnvir *)cde;

/**
 Remove item.
 */
- (void)removeFrom:(CoreDataEnvir *)cde;

/**
 Save db on main thread.
 */
- (BOOL)saveTo:(CoreDataEnvir *)cde;

@end




CoreDataEnvir (2011-05-25)
=============

A CoreData Envirement wrapper, use CoreData in convient way. You can use it in concurrency，run seperate CoreDataEnvir instance on one thread.

##First step:
	//Register your data base file name
	[CoreDataEnvir registDatabaseFileName:@"db.sqlite"];
	//Register your model file name(no extension)
	[CoreDataEnvir registModelFileName:@"SampleModel"];


##On main thread:

#Insert item:
	Team *team = [Team insertItem];
	or
	[Team insertItemWith:^(Team *item) {
		item.name = [NSString stringWithFormat:@"Nicholas"];
	}];
	
	[team save];//Save to data base on main thread.
	or
	[team remove];//Remove from data on main thread.

#Fetch one item:
	Team *team = (Team *)[Team lastItemWith:[NSPredicate predicateWithFormat:@"name==nicholas"]];

#Fetch all items:
	NSArray *allItems = [Team items];

##If you want to run on other threads, follow this:

This method ([CoreDataEnvir instance]) will automatically creating new instance for non-main thread and uniq instance for main thread.

	dispatch_async(q2, ^{
		CoreDataEnvir *db = [CoreDataEnvir instance];
		for (int i = 0; i < 500; i++) {
			Team *team = (Team *)[Team lastItemWith:db predicate:[NSPredicate predicateWithFormat:@"name==9"]];
			if (team) {
				[team removeFrom:db];//Remove from db.
			}else {
				[Team insertItemWith:db fillData:^(Team *item) {
					item.name = [NSString stringWithFormat:@"9"];
				}];
				
			}
			[team saveTo:db];//Save db.
		}
	});

##If your are newcomer to CoreData, please obey the rules below:
If you wanna keep you NSManagedObject objects, you shouldn't release you CoreDataEnvir object or it will be fault. So if you operate data base in multiple threads, make sure your NSManagedObject object reference fetched from [CoreDataEnvir mainInstance] or [CoreDataEnvir backgroundInstance] which never be released until application exist.


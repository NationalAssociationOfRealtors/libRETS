var librets = require("./build/Release/librets.node");

function dump_system(metadata)
{
    var system = metadata.GetSystem();
    console.log("System ID: " + system.GetSystemID());
    console.log("Description: " + system.GetSystemDescription());
    console.log("Comments: " + system.GetComments());
}

function dump_all_resources(metadata)
{
    var resources = metadata.GetAllResources();
    for (var i = 0; i < resources.size(); i++)
    {
	dump_all_classes(metadata, resources.get(i));
    	dump_all_lookups(metadata, resources.get(i));
    }
}

function dump_all_classes(metadata, resource)
{
    var resourceName = resource.GetResourceID();

    var classes = metadata.GetAllClasses(resourceName);
    for (var i = 0; i < classes.size(); i++)
    {
	var aClass = classes.get(i);
	console.log("Resource name: " + resourceName + " [" + resource.GetStandardName() + "]");
	console.log("Class name: " + aClass.GetClassName() + " [" + aClass.GetStandardName() + "]");
	dump_all_tables(metadata, aClass);
	console.log();
    }
}

function dump_all_lookups(metadata, resource)
{
    var resourceName = resource.GetResourceID();

    var lookups = metadata.GetAllLookups(resourceName);
    for (var i = 0; i < lookups.size(); i++)
    {
	var lookup = lookups.get(i);
	console.log("Resource name: " + resourceName + " [" + resource.GetStandardName() + "]");
	console.log("Lookup name: " + lookup.GetLookupName() + " (" + lookup.GetVisibleName() + ")");

	//GetMetadataEntryID doesn't look to be exported by swig
	// if (!lookup.GetMetadataEntryID().empty())
	// {
	//     console.log(" MetadataEntryID: " + lookup.GetMetadataEntryID());
	// }
	dump_all_lookup_types(metadata, lookup);
	console.log();
    }
}

function dump_all_lookup_types(metadata, lookup)
{
    var lookupTypes = metadata.GetAllLookupTypes(lookup);
    for (var i = 0; i < lookupTypes.size(); i++)
    {
	lt = lookupTypes.get(i);
	console.log("Lookup value: " + lt.GetValue() + " (" +
		    lt.GetShortValue() + ", " +
		    lt.GetLongValue() + ")");
    }
}

function dump_all_tables(metadata, aClass)
{
    var tables = metadata.GetAllTables(aClass);
    for (var i = 0; i < tables.size(); i++)
    {
	var table = tables.get(i);
	console.log("Table name: " + table.GetSystemName() + " ["
		    + table.GetStandardName() + "]" + " ("
		    + table.GetDataType() + ")");
	//GetMetadataEntryID doesn't look to be exported by swig
	// if (!table.GetMetadataEntryID().empty())
        // {
        //     console.log(" MetadataEntryID: " + table.GetMetadataEntryID());
        // }
        if (table.InKeyIndex())
        {
            console.log(" InKeyIndex");
        }
    }
}

try
{
    var session = new librets.RetsSession("http://www.dis.com:6103/rets/login");

    if (!session.Login("Joe", "Schmoe"))
    {
	console.log("Login failed");
	process.exit(-1);
    }

    var metadata = session.GetMetadata();
    dump_system(metadata);
    // TODO: dump_all_foreign_keys
    dump_all_resources(metadata);

    session.Logout();
}
catch (e)
{
    console.log("Exception: " + e);
}

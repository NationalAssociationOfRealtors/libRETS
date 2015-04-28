var util = require('util');
var librets = require("./build/Release/librets.node");

function dump_system(metadata)
{
    var system = metadata.GetSystem();
    console.log(util.format("System ID: %s\n", system.GetSystemID()));
    console.log(util.format('Description: %s\n', system.GetSystemDescription()));
    console.log(util.format("Comments: %s\n", system.GetComments()));
}

function dump_all_resources(metadata)
{
    var resources = metadata.GetAllResources();
    console.log("");
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
	console.log(util.format("Resource name: %s [%s]\n", resourceName,
			     resource.GetStandardName()));
	console.log(util.format("Class name: %s [%s]\n", aClass.GetClassName(),
			     aClass.GetStandardName()));
	dump_all_tables(metadata, aClass);
	console.log("");
    }
}

function dump_all_lookups(metadata, resource)
{
    var resourceName = resource.GetResourceID();

    var lookups = metadata.GetAllLookups(resourceName);
    for (var i = 0; i < lookups.size(); i++)
    {
	var lookup = lookups.get(i);
	console.log(util.format("Resource name: %s [%s]\n", resourceName,
			       resource.GetStandardName()));
	console.log(util.format("Lookup name: %s (%s)", lookup.GetLookupName(),
			       lookup.GetVisibleName()));

	//GetMetadataEntryID doesn't look to be exported by swig
	// if (!lookup.GetMetadataEntryID().empty())
	// {
	//     console.log(util.format(" MetadataEntryID: %s",
	//                lookup.GetMetadataEntryID()));
	// }
	console.log("")
	dump_all_lookup_types(metadata, lookup);
	console.log("");
    }
}

function dump_all_lookup_types(metadata, lookup)
{
    var lookupTypes = metadata.GetAllLookupTypes(lookup);
    for (var i = 0; i < lookupTypes.size(); i++)
    {
	lt = lookupTypes.get(i);
	console.log(util.format("Lookup value: %s (%s, %s)\n", lt.GetValue(),
			       lt.GetShortValue(), lt.GetLongValue()));
    }
}

function dump_all_tables(metadata, aClass)
{
    var tables = metadata.GetAllTables(aClass);
    for (var i = 0; i < tables.size(); i++)
    {
	var table = tables.get(i);
	console.log(util.format("Table name: %s [%s] (%d)",
			       table.GetSystemName(), table.GetStandardName(),
			       table.GetDataType()));
	//GetMetadataEntryID doesn't look to be exported by swig
	// if (!table.GetMetadataEntryID().empty())
        // {
        //     console.log(util.format(" MetadataEntryID: " + table.GetMetadataEntryID());
        // }
        if (table.InKeyIndex())
        {
            console.log(" InKeyIndex");
        }
	console.log("");
    }
}

function dump_all_foreign_keys(metadata)
{
    var foreign_keys = metadata.GetAllForeignKeys();
    console.log("");
    for (var i = 0; i < foreign_keys.size(); i++)
    {
	var fk = foreign_keys.get(i);
	console.log(util.format("Foreign Key ID: %s\n", fk.GetForeignKeyID()));
        console.log(util.format("  Parent Resource: %s",
			       fk.GetParentResourceID()));
        console.log(util.format(", Class: %s", fk.GetParentClassID()));
        console.log(util.format(", Name: %s\n", fk.GetParentSystemName()));
        console.log(util.format("  Child Resource: %s",
			       fk.GetChildResourceID()));
        console.log(util.format(", Class: %s", fk.GetChildClassID()));
        console.log(util.format(", Name: %s\n", fk.GetChildSystemName()));
    }
}

try
{
    var session = new librets.RetsSession("http://www.dis.com:6103/rets/login");

    session.SetHttpLogName("trace.txt");                                         
    session.SetRetsVersion(librets.RETS_1_8_0);                                  

    if (!session.Login("Joe", "Schmoe"))
    {
	console.log("Login failed");
	process.exit(-1);
    }

    var metadata = session.GetMetadata();
    dump_system(metadata);
    dump_all_foreign_keys(metadata);
    dump_all_resources(metadata);

    session.Logout();
}
catch (e)
{
    console.log("Exception: " + e);
}

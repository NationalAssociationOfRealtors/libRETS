var util = require('util');
var librets = require("./build/Release/librets.node");

function dump_system(metadata)
{
    var system = metadata.GetSystem();
    util.print(util.format("System ID: %s\n", system.GetSystemID()));
    util.print(util.format('Description: %s\n', system.GetSystemDescription()));
    util.print(util.format("Comments: %s\n", system.GetComments()));
}

function dump_all_resources(metadata)
{
    var resources = metadata.GetAllResources();
    util.puts("");
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
	util.print(util.format("Resource name: %s [%s]\n", resourceName,
			     resource.GetStandardName()));
	util.print(util.format("Class name: %s [%s]\n", aClass.GetClassName(),
			     aClass.GetStandardName()));
	dump_all_tables(metadata, aClass);
	util.puts("");
    }
}

function dump_all_lookups(metadata, resource)
{
    var resourceName = resource.GetResourceID();

    var lookups = metadata.GetAllLookups(resourceName);
    for (var i = 0; i < lookups.size(); i++)
    {
	var lookup = lookups.get(i);
	util.print(util.format("Resource name: %s [%s]\n", resourceName,
			       resource.GetStandardName()));
	util.print(util.format("Lookup name: %s (%s)", lookup.GetLookupName(),
			       lookup.GetVisibleName()));

	//GetMetadataEntryID doesn't look to be exported by swig
	// if (!lookup.GetMetadataEntryID().empty())
	// {
	//     util.print(util.format(" MetadataEntryID: %s",
	//                lookup.GetMetadataEntryID()));
	// }
	util.puts("")
	dump_all_lookup_types(metadata, lookup);
	util.puts("");
    }
}

function dump_all_lookup_types(metadata, lookup)
{
    var lookupTypes = metadata.GetAllLookupTypes(lookup);
    for (var i = 0; i < lookupTypes.size(); i++)
    {
	lt = lookupTypes.get(i);
	util.print(util.format("Lookup value: %s (%s, %s)\n", lt.GetValue(),
			       lt.GetShortValue(), lt.GetLongValue()));
    }
}

function dump_all_tables(metadata, aClass)
{
    var tables = metadata.GetAllTables(aClass);
    for (var i = 0; i < tables.size(); i++)
    {
	var table = tables.get(i);
	util.print(util.format("Table name: %s [%s] (%d)",
			       table.GetSystemName(), table.GetStandardName(),
			       table.GetDataType()));
	//GetMetadataEntryID doesn't look to be exported by swig
	// if (!table.GetMetadataEntryID().empty())
        // {
        //     util.print(util.format(" MetadataEntryID: " + table.GetMetadataEntryID());
        // }
        if (table.InKeyIndex())
        {
            util.print(" InKeyIndex");
        }
	util.puts("");
    }
}

function dump_all_foreign_keys(metadata)
{
    var foreign_keys = metadata.GetAllForeignKeys();
    util.puts("");
    for (var i = 0; i < foreign_keys.size(); i++)
    {
	var fk = foreign_keys.get(i);
	util.print(util.format("Foreign Key ID: %s\n", fk.GetForeignKeyID()));
        util.print(util.format("  Parent Resource: %s",
			       fk.GetParentResourceID()));
        util.print(util.format(", Class: %s", fk.GetParentClassID()));
        util.print(util.format(", Name: %s\n", fk.GetParentSystemName()));
        util.print(util.format("  Child Resource: %s",
			       fk.GetChildResourceID()));
        util.print(util.format(", Class: %s", fk.GetChildClassID()));
        util.print(util.format(", Name: %s\n", fk.GetChildSystemName()));
    }
}

try
{
    var session = new librets.RetsSession("http://www.dis.com:6103/rets/login");

    if (!session.Login("Joe", "Schmoe"))
    {
	util.puts("Login failed");
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

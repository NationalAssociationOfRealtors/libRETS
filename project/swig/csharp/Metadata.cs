using System;
using System.Collections;
using librets;

public class Search
{
    static void Main(string[] args)
    {
        RetsSession session = new RetsSession(
            "http://demo.crt.realtors.org:6103/rets/login");
        if ((args.Length == 1) && args[0].Equals("full"))
        {
            session.SetIncrementalMetadata(false);
        }
        
        if (!session.Login("Joe", "Schmoe"))
        {
            Console.WriteLine("Invalid login");
            Environment.Exit(1);
        }

        RetsMetadata metadata = session.GetMetadata();
        dumpSystem(metadata);
        dumpAllResources(metadata);

        session.Logout();
    }
    
    static void dumpSystem(RetsMetadata metadata)
    {
        MetadataSystem system = metadata.GetSystem();
        Console.WriteLine("System ID: " + system.GetSystemID());
        Console.WriteLine("Description: " + system.GetSystemDescription());
        Console.WriteLine("Comment: " + system.GetComments());
    }
    
    static void dumpAllResources(RetsMetadata metadata)
    {
        Console.WriteLine();
        IEnumerable resources = metadata.GetAllResources();
        foreach (MetadataResource resource in resources)
        {
            dumpAllClasses(metadata, resource);
        }
        foreach (MetadataResource resource in resources)
        {
            dumpAllLookups(metadata, resource);
        }
    }
    
    static void dumpAllClasses(RetsMetadata metadata,
        MetadataResource resource)
    {
        string resourceName = resource.GetResourceID();
        IEnumerable classes = metadata.GetAllClasses(resourceName);
        foreach (MetadataClass aClass in classes)
        {
            Console.WriteLine("Resource name: " + resourceName + " [" +
                resource.GetStandardName() + "]");
            Console.WriteLine("Class name: " + aClass.GetClassName() + " [" +
                aClass.GetStandardName() + "]");
            dumpAllTables(metadata, aClass);
            Console.WriteLine();
        }
    }
    
    static void dumpAllTables(RetsMetadata metadata, MetadataClass aClass)
    {
        IEnumerable tables = metadata.GetAllTables(aClass);
        foreach (MetadataTable table in tables)
        {
            Console.WriteLine("Table name: " + table.GetSystemName() + " [" +
                table.GetStandardName() + "]");
            Console.WriteLine("\tTable datatype: " + table.GetDataType());
            Console.WriteLine("\tUnique: " + table.IsUnique());
            Console.WriteLine("\tMax Length: " + table.GetMaximumLength());
        }
    }

    static void dumpAllLookups(RetsMetadata metadata,
        MetadataResource resource)
    {
        string resourceName = resource.GetResourceID();
        IEnumerable lookups = metadata.GetAllLookups(resourceName);
        foreach (MetadataLookup lookup in lookups)
        {
            Console.WriteLine("Resource name: " + resourceName + " [" +
                resource.GetStandardName() + "]");
            Console.WriteLine("Lokup name: " + lookup.GetLookupName() + " (" +
                lookup.GetVisibleName() + ")");
            dumpAllLookupTypes(metadata, lookup);
            Console.WriteLine();
        }
    }
    
    static void dumpAllLookupTypes(RetsMetadata metadata,
        MetadataLookup lookup)
    {
        IEnumerable lookupTypes = metadata.GetAllLookupTypes(lookup);
        foreach (MetadataLookupType lookupType in lookupTypes)
        {
            Console.WriteLine("Lookup value: " + lookupType.GetValue() +
                " (" + lookupType.GetShortValue() + ", " +
                lookupType.GetLongValue() + ")");
        }
    }
}

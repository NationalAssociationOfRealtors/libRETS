import java.lang.*;
import java.util.*;
import librets.*;

public class metadata 
{
    static
    {
        try
        {
            System.loadLibrary("librets");

        }
        catch (UnsatisfiedLinkError e)
        {
            System.err.println("Unable to load the librets native library.\n" + e);
            System.exit(1);
        }
    }
    

    public static void main(String[] args)
    {
        RetsSession session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

        if (args.length == 1 && args[0].equals("full"))
        {
            session.SetIncrementalMetadata(false);
        }

        if (!session.Login("Joe", "Schmoe"))
        {
            System.out.println("Invalid login");
            System.exit(2);
        }

        RetsVersion version = session.GetDetectedRetsVersion();

        System.out.println("RETS Version: " +
            ((version == RetsVersion.RETS_1_5) ? "1.5" : "1.0"));

        RetsMetadata metadata = session.GetMetadata();
        dumpSystem(metadata);
        dumpAllForeignKeys(metadata);
        dumpAllResources(metadata);
        
        LogoutResponse logout = session.Logout();

        System.out.println("Billing info: " + logout.GetBillingInfo());
        System.out.println("Logout Message:  " + logout.GetLogoutMessage());
        System.out.println("Connect time: " + logout.GetConnectTime());
    }

    static void dumpAllForeignKeys(RetsMetadata metadata)
    {
        MetadataForeignKeyList foreignKeys = metadata.GetAllForeignKeys();
        for (int i = 0; i < foreignKeys.size(); i++)
        {
            MetadataForeignKey foreignKey = foreignKeys.get(i);
            System.out.println ("Foreign Key ID: " + foreignKey.GetForeignKeyID());
            System.out.print  ("  Parent Resource: " + foreignKey.GetParentResourceID());
            System.out.print  (", Class: " + foreignKey.GetParentClassID());
            System.out.println (", Name: " + foreignKey.GetParentSystemName());
            System.out.print  ("  Child Resource: " + foreignKey.GetChildResourceID());
            System.out.print  (", Class: " + foreignKey.GetChildClassID());
            System.out.println (", Name: " + foreignKey.GetChildSystemName());
        }
    }

    static void dumpAllClasses(RetsMetadata metadata, MetadataResource resource)
    {
        String resourceName = resource.GetResourceID();
        MetadataClassList classes = metadata.GetAllClasses(resourceName);

        for (int i = 0; i < classes.size(); i++)
        {
            MetadataClass aClass = classes.get(i);
            
            System.out.println("Resource name: " + resourceName + " [" +
                    resource.GetStandardName() + "]");
            System.out.println("Class name: " + aClass.GetClassName() + " [" +
                    aClass.GetStandardName() + "]");
            dumpAllTables(metadata, aClass);
            System.out.println();

        }
    }

    static void dumpAllLookups(RetsMetadata metadata, MetadataResource resource)
    {
        String resourceName = resource.GetResourceID();
        MetadataLookupList lookups = metadata.GetAllLookups(resourceName);
        for (int i = 0; i < lookups.size(); i++)
        {
            MetadataLookup lookup = lookups.get(i);
            System.out.println("Resource name: " + resourceName + " [" +
                resource.GetStandardName() + "]");
            System.out.println("Lokup name: " + lookup.GetLookupName() + " (" +
                lookup.GetVisibleName() + ")");
            dumpAllLookupTypes(metadata, lookup);
            System.out.println();
        }
    }

    static void dumpAllLookupTypes(RetsMetadata metadata, MetadataLookup lookup)
    {
        MetadataLookupTypeList lookupTypes = metadata.GetAllLookupTypes(lookup);

        for (int i = 0; i < lookupTypes.size(); i++)
        {
            MetadataLookupType lookupType = lookupTypes.get(i);
            System.out.println("Lookup value: " + lookupType.GetValue() +
                        " (" + lookupType.GetShortValue() + ", " +
                        lookupType.GetLongValue() + ")");
        }
    }

    static void dumpAllResources(RetsMetadata metadata)
    {
        System.out.println();

        MetadataResourceList resources = metadata.GetAllResources();

        for (int i = 0; i < resources.size(); i++)
        {
            MetadataResource resource = resources.get(i);
            dumpAllClasses(metadata, resource);
        }

        for (int i = 0; i < resources.size(); i++)
        {
            MetadataResource resource = resources.get(i);
            dumpAllLookups(metadata, resource);
        }
    }

    static void dumpAllTables(RetsMetadata metadata, MetadataClass aClass)
    {
        MetadataTableList tables = metadata.GetAllTables(aClass);
        for (int i = 0; i < tables.size(); i++)
        {
            MetadataTable table = tables.get(i);
            System.out.println("Table name: " + table.GetSystemName() + " [" +
                    table.GetStandardName() + "]");
            System.out.println("\tTable datatype: " + table.GetDataType());
            System.out.println("\tUnique: " + table.IsUnique());
            System.out.println("\tMax Length: " + table.GetMaximumLength());
            if (table.InKeyIndex())
                System.out.println("\tInKeyIndex");
        }
    }

    static void dumpSystem(RetsMetadata metadata)
    {
        MetadataSystem system = metadata.GetSystem();

        System.out.println("System ID: " + system.GetSystemID());
        System.out.println("Description: " + system.GetSystemDescription());
        System.out.println("Comment: " + system.GetComments());
    }

}

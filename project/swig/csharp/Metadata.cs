using System;
using System.Collections;
using librets;

public class Search
{
    static void Main(string[] args)
    {
        RetsSession session = new RetsSession(
            "http://demo.crt.realtors.org:6103/rets/login");
        session.Login("Joe", "Schmoe");

        RetsMetadata metadata = session.GetMetadata();
        MetadataSystem system = metadata.GetSystem();
        Console.WriteLine("System ID: " + system.GetSystemID());
        Console.WriteLine("Description: " + system.GetSystemDescription());
        Console.WriteLine("Comment: " + system.GetComments());
        
        IEnumerable resources = metadata.GetAllResources();
        foreach (MetadataResource resource in resources) {
            Console.WriteLine(resource.GetStandardName());
        }

        session.Logout();
    }
}

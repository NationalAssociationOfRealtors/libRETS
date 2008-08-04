using System;
using System.Collections;
using System.IO;
using System.Collections.Specialized;
using librets;

public class Search
{
    static void Main(string[] args)
    {
        Options options  = new Options();

        if (!options.Parse(args))
	    Environment.Exit(1);

        RetsSession session = options.SessionFactory();

        if (!session.Login(options.user_name, options.user_password))
        {
            Console.WriteLine("Invalid login");
            Environment.Exit(1);
        }

        Console.WriteLine("Action: " + session.GetAction());
        RetsVersion version = session.GetDetectedRetsVersion();
        Console.WriteLine("RETS Version: " +
            ((version == RetsVersion.RETS_1_5) ? "1.5" : 
            ((version == RetsVersion.RETS_1_7) ? "1.7" : "1.0")));

        SearchRequest searchRequest = session.CreateSearchRequest(
	    options.search_type, options.search_class, options.query);

        searchRequest.SetSelect(options.select);
        searchRequest.SetLimit(options.limit);
        searchRequest.SetOffset(options.offset);
        searchRequest.SetCountType(options.count);
        searchRequest.SetStandardNames(options.standard_names);
        SearchResultSet results = session.Search(searchRequest);
        
        Console.WriteLine("Record count: " + results.GetCount());
        Console.WriteLine();
        IEnumerable columns = results.GetColumns();
        while (results.HasNext())
        {
            foreach (string column in columns)
            {
                Console.WriteLine(column + ": " + results.GetString(column));
            }
            Console.WriteLine();
        }

        LogoutResponse logout = session.Logout();
        Console.WriteLine("Billing info: " + logout.GetBillingInfo());
        Console.WriteLine("Logout message: " + logout.GetLogoutMessage());
        Console.WriteLine("Connect time: " + logout.GetConnectTime());
    }

    
}

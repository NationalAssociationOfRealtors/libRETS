using System;
using System.Collections;
using System.IO;
using System.Collections.Specialized;
using librets;

public class RawSearch
{
    static void Main(string[] args)
    {
        Options options  = new Options();

        if (!options.Parse(args))
            Environment.Exit(1);

        RetsSession session = options.SessionFactory();

        try {

        if (!session.Login(options.user_name, options.user_password))
        {
            Console.WriteLine("Invalid login");
            Environment.Exit(1);
        }
        } catch (Exception e)
        {
            Console.WriteLine("RetsException: " + e);
            Environment.Exit(1);
        }

        SearchRequest searchRequest = session.CreateSearchRequest(
            options.search_type, options.search_class, options.query);

        searchRequest.SetSelect(options.select);
        searchRequest.SetLimit(options.limit);
        searchRequest.SetOffset(options.offset);
        searchRequest.SetCountType(options.count);
        searchRequest.SetStandardNames(options.standard_names);

        try
        {
            Stream outputStream = File.OpenWrite("rawsearch.xml");
            byte [] data = session.SearchAsArray(searchRequest);
            outputStream.Write(data, 0, data.Length);
            outputStream.Close();
        } 
        catch (Exception e)
        {
            Console.WriteLine("RetsException: " + e);
            session.Logout();
            Environment.Exit(1);
        }
        
        session.Logout();

        try
        {
            // I'd rather use ReadAllBytes, but it doesn't exist in all versions.
            // byte [] data = File.ReadAllBytes("rawsearch.xml"); 
            //
            FileInfo info = new FileInfo("rawsearch.xml");
            int len = (int)info.Length;
            byte [] data = new byte [len];
            FileStream inputStream = info.OpenRead();
            inputStream.Read(data,0,len);

            SearchResultSet results = new SearchResultSet();

            results.SetEncoding(options.encoding);
            results.SetDataAsArray(data);

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
        } 
        catch (Exception e)
        {
            Console.WriteLine("RetsException: " + e);
            session.Logout();
            Environment.Exit(1);
        }
    }

    
}

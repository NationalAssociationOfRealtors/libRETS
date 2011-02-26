using System;
using System.Collections;
using System.IO;
using System.Collections.Specialized;
using librets;

public class Interleaved
{
    /*
     * This class demonstrates the interleaving of search transactions.
     */
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

        RetsVersion version = session.GetDetectedRetsVersion();
        Console.WriteLine("RETS Version: " +
            ((version == RetsVersion.RETS_1_5) ? "1.5" : 
            ((version == RetsVersion.RETS_1_7) ? "1.7" : "1.0")));

        /*
         * Find the key field for the resource.
         */
        RetsMetadata metadata = session.GetMetadata();
        MetadataResource metadataResource = metadata.GetResource(options.search_type);

        if (metadataResource == null)
        {
            Console.WriteLine("Invalid resource: " + options.search_type);
            session.Logout();
            Environment.Exit(1);
        }

        string keyField = metadataResource.GetKeyField();

        /*
         * Find the timestamp field if it is known (RETS 1.7 and later). If
         * not known, then the user must provide it.
         */
        MetadataClass metadataClass = metadata.GetClass(options.search_type, options.search_class);
        if (metadataClass == null)
        {
            Console.WriteLine("Invalid resource:class: " + options.search_type + ":" + options.search_class);
            session.Logout();
            Environment.Exit(2);
        }

        if (options.classTimeStamp != null && options.classTimeStamp.Length == 0)
            options.classTimeStamp = metadataClass.GetStringAttribute("ClassTimeStamp");

        if (options.classTimeStamp == null || options.classTimeStamp.Length == 0)
        {
            Console.WriteLine("Class " + options.search_type +
                                  ":" + options.search_class +
                              " has no ClassTimeStamp specified in the metadata.");
            Console.WriteLine("Please manually provide one using the --timetsamp switch.");
            session.Logout();
            Environment.Exit(2);
        }
        
        /*
         * See if the last modified timestamp has been provided. If not, use yesterday.
         */
        if (options.lastModified == null || options.lastModified.Length == 0)
        {
            DateTime ts = DateTime.Now;

            options.lastModified = ts.AddDays(-1).ToString("yyyy-MM-dd");
        }

        /*
         * OK - let's find all listings that have changed since the lastModified date.
         */
        
        SearchRequest searchRequest = session.CreateSearchRequest(
                                                options.search_type, 
                                                options.search_class,
                                                "(" + 
                                                options.classTimeStamp.ToString() + 
                                                "=" +
                                                options.lastModified.ToString() +
                                                "+)");

        searchRequest.SetSelect(keyField);
        searchRequest.SetLimit(SearchRequest.LIMIT_NONE);
        searchRequest.SetOffset(SearchRequest.OFFSET_NONE);
        searchRequest.SetCountType(SearchRequest.CountType.RECORD_COUNT_AND_RESULTS);
        searchRequest.SetStandardNames(false);

        /*
         * This starts the outer search. 
         */
        SearchResultSet results = session.Search(searchRequest);
        
        Console.WriteLine("Record count: " + results.GetCount());
        Console.WriteLine();

        while (results.HasNext())
        {
            /*
             * Fetch the listing detail and media. This will cause a separate search transaction
             * to be open within the outer search transaction.
             */
            SearchRequest listingRequest = session.CreateSearchRequest(
                                                            options.search_type,
                                                            options.search_class,
                                                            "(" +
                                                            keyField +
                                                            "=" +
                                                            results.GetString(keyField) +
                                                            ")");
            listingRequest.SetStandardNames(false);
            listingRequest.SetLimit(SearchRequest.LIMIT_DEFAULT);
            listingRequest.SetOffset(SearchRequest.OFFSET_NONE);
            listingRequest.SetCountType(SearchRequest.CountType.NO_RECORD_COUNT);
            listingRequest.SetFormatType(SearchRequest.FormatType.COMPACT);
            
            SearchResultSet listingResult = session.Search(listingRequest);
            IEnumerable  columns = null;
            
            while (listingResult.HasNext())
            {
                if (columns == null)
                {
                    columns = listingResult.GetColumns();
                }
                /*
                 * Show the listing detail.
                 */
                foreach (string column in columns)
                {
                    Console.WriteLine("{0,15}: {1}", column, listingResult.GetString(column));
                }
                Console.WriteLine();
            
                /*
                 * Now set up to fetch the objects associated with this listing.
                 */
                GetObjectRequest getObjectRequest = new GetObjectRequest(options.search_type, "Photo");
                getObjectRequest.AddAllObjects(listingResult.GetString(keyField));
                
                GetObjectResponse getObjectResponse = session.GetObject(getObjectRequest);
                
                foreach(ObjectDescriptor objectDescriptor in getObjectResponse)
                {
                    /*
                     * Report the object details.
                     */
                    string  objectKey   = objectDescriptor.GetObjectKey();
                    int     objectId    = objectDescriptor.GetObjectId();
                    //string  contentType = objectDescriptor.GetContentType();
                    string  description = objectDescriptor.GetDescription();
                    
                    Console.Write ("Object " + objectKey + ":" + objectId.ToString());
                    if (description.Length > 0)
                        Console.Write (", description: " + description);
                    Console.WriteLine();
                }
                Console.WriteLine("=================");
            }
        }

        session.Logout();
    }

    
}

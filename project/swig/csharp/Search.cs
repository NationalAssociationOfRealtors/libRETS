using System;
using librets;

public class Search
{
    static void Main(string[] args)
    {
        RetsSession session = new RetsSession(
            "http://demo.crt.realtors.org:6103/rets/login");
        session.Login("Joe", "Schmoe");

        SearchRequest searchRequest = session.CreateSearchRequest(
            "Property", "ResidentialProperty", "(ListPrice=300000-)");

        searchRequest.SetSelect("ListingID,ListPrice,Beds,City");
        searchRequest.SetLimit(SearchRequest.LIMIT_DEFAULT);
        searchRequest.SetOffset(SearchRequest.OFFSET_NONE);
        searchRequest.SetCountType(SearchRequest.CountType.RECORD_COUNT_AND_RESULTS);
        SearchResultSet results = session.Search(searchRequest);
        
        Console.WriteLine("Record count: " + results.GetCount());
        Console.WriteLine();
        StringVector columns = results.GetColumns();
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

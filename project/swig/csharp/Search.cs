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

        SearchResultSet results = session.Search(searchRequest);
        while (results.HasNext())
        {
            Console.WriteLine("ListingID: " +
                              results.GetString("ListingID"));
            Console.WriteLine("ListPrice: " +
                              results.GetString("ListPrice"));
            Console.WriteLine("Beds: " +
                              results.GetString("Beds"));
            Console.WriteLine("City: " + results.GetString("City"));
            Console.WriteLine();
        }

        LogoutResponse logout = session.Logout();
        Console.WriteLine("Billing info: " + logout.GetBillingInfo());
        Console.WriteLine("Logout message: " + logout.GetLogoutMessage());
        Console.WriteLine("Connect time: " + logout.GetConnectTime());
    }
}

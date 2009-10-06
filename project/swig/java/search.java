import java.lang.*;
import librets.*;
import java.io.*;

public class search 
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
    

    public static void main(String argv[])
    {
        RetsSession session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");

        if (!session.Login("Joe", "Schmoe"))
        {
            System.out.println("Invalid login");
            System.exit(2);
        }

        System.out.println("Action: " + session.GetAction());
        RetsVersion version = session.GetDetectedRetsVersion();

        System.out.println("RETS Version: " +
            ((version == RetsVersion.RETS_1_5) ? "1.5" : "1.0"));

        SearchRequest searchRequest= session.CreateSearchRequest(
            "Property", "RES", "(ListPrice=300000-)");

        searchRequest.SetSelect("ListingID,ListPrice,Beds,City");
        searchRequest.SetLimit(SearchRequest.LIMIT_DEFAULT);
        searchRequest.SetOffset(SearchRequest.OFFSET_NONE);
        searchRequest.SetCountType(SearchRequest.CountType.RECORD_COUNT_AND_RESULTS);
        searchRequest.SetStandardNames(true);
        searchRequest.SetRestrictedIndicator("XXXX");
        SearchResultSet results = session.Search(searchRequest);

        System.out.println("Record count: " + results.GetCount());

        StringVector columns = results.GetColumns();

        while (results.HasNext())
        {
            for (int i = 0; i < columns.size(); i++)
            {
                System.out.format("%15s: %s\n", columns.get(i), results.GetString(columns.get(i)));
            }
            System.out.println();
        }
        
        LogoutResponse logout = session.Logout();

        System.out.println("Billing info: " + logout.GetBillingInfo());
        System.out.println("Logout Message:  " + logout.GetLogoutMessage());
        System.out.println("Connect time: " + logout.GetConnectTime());
    }
}

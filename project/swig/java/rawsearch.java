import java.lang.*;
import librets.*;
import java.io.*;

public class rawsearch 
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
    

    public static void main(String argv[]) throws Exception
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
        searchRequest.SetFormatType(SearchRequest.FormatType.COMPACT);
        searchRequest.SetStandardNames(true);

	try
	{
	    File f=new File("rawsearch.xml");
	    FileOutputStream fop=new FileOutputStream(f);
	    byte [] data = session.SearchAsArray(searchRequest);
	    fop.write(data);
	    fop.flush();
	    fop.close();
	}
	catch (IOException e) {}

        LogoutResponse logout = session.Logout();

	SearchResultSet results = new SearchResultSet();

	// Reopen the file now for input
	try
	{
	    File f = new File("rawsearch.xml");
	    byte [] buffer = new byte[(int)f.length()];

	    FileInputStream fip=new FileInputStream(f);

	    int offset = 0;
	    int numRead = 0;

	    while (offset < buffer.length && (numRead=fip.read(buffer, offset, buffer.length - offset)) >= 0)
	    	offset += numRead;

	    results.SetEncoding(EncodingType.RETS_XML_DEFAULT_ENCODING);
	    results.SetDataAsArray(buffer);
	}
	catch (IOException e) {}

        System.out.println("Record count: " + results.GetCount());

        StringVector columns = results.GetColumns();

        while (results.HasNext())
        {
            if (columns == null)
            {
                columns = results.GetColumns();
            }
            for (int i = 0; i < columns.size(); i++)
            {
                System.out.format("%15s: %s\n", columns.get(i), results.GetString(columns.get(i)));
            }
            System.out.println();
        }
        
	/*
	 * Prototype for returning data in a stream.
	try
	{
	    File f=new File("foobarty");
	    FileOutputStream fop=new FileOutputStream(f);
	    CppInputStream data = session.SearchAsStream(searchRequest);
	    byte [] buf = new byte[30];
	    int len;
	    while ((len = data.read(buf, 0, 30)) > 0)
	    {
	        fop.write(buf, 0, len);
	    }
	    fop.flush();
	    fop.close();
	}
	catch (IOException e) {}
	 * end prototype */

    }
}

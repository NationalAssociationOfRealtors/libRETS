import java.lang.*;
import java.util.*;
import java.io.*;
import librets.*;

public class rawmetadata 
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

        if (!session.Login("Joe", "Schmoe"))
        {
            System.out.println("Invalid login");
            System.exit(2);
        }

        RetsVersion version = session.GetDetectedRetsVersion();

        System.out.println("RETS Version: " +
            ((version == RetsVersion.RETS_1_5) ? "1.5" : "1.0"));

	try
	{
	    File f=new File("rawmetadata.log");
	    FileOutputStream fop=new FileOutputStream(f);
	    byte [] data = session.GetMetadataAsArray();
	    fop.write(data);
	    fop.flush();
	    fop.close();
	}
	catch (IOException e) {}
        
        LogoutResponse logout = session.Logout();

        System.out.println("Billing info: " + logout.GetBillingInfo());
        System.out.println("Logout Message:  " + logout.GetLogoutMessage());
        System.out.println("Connect time: " + logout.GetConnectTime());
    }
}

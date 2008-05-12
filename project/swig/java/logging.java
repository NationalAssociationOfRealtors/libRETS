import java.io.*;
import java.lang.*;
import librets.*;

public class logging
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
    
    public static class TestLogger extends RetsHttpLogger
    {
	Type last_type = Type.INFORMATIONAL;
        PrintWriter logfile = null;

        public TestLogger(String filename)
	{
	    try
	    {
	        logfile = new PrintWriter(new FileWriter(filename));
	    }
	    catch (Exception e)
	    {
	      System.err.println("Catch!");
	    }
	}

	protected void finalize() 
	{
	    System.out.println("Closing logfile");
	    if (logfile != null)
	    {
	      logfile.flush();
	      logfile.close();
	    }
	    super.finalize();
	}

	public void logHttpData(Type type, String data)
	{
	    logfile.println();
	    if (type == Type.RECEIVED && last_type != Type.RECEIVED)
	    {
		logfile.println("<<< Received");
	    }
	    else
	    if (type == Type.SENT && last_type != Type.SENT)
	    {
		logfile.println(">>> Sent");
	    }
	    else
	    if (type == Type.INFORMATIONAL)
	    {
	        logfile.print("* ");
	    }
	    logfile.print(data);
	    last_type = type;
	}
    }


    public static void main(String[] args)
    {
	RetsSession session = new RetsSession("http://demo.crt.realtors.org:6103/rets/login");
	TestLogger logger = null;

	if (args.length > 0)
	    logger = new TestLogger(args[0]);

	try
	{
	    session.SetHttpLogger(logger);

	    if (!session.Login("Joe", "Schmoe"))
	    {
		System.out.println("Invalid login");
		System.exit(2);
	    }

	    LoginResponse response = session.GetLoginResponse();
	    CapabilityUrls capurls = session.GetCapabilityUrls();

	    System.out.println("Member name: " + response.GetMemberName());
	    System.out.println("Search URL: " + capurls.GetSearchUrl());

	    String version = "1.0";
	    if (session.GetDetectedRetsVersion() == RetsVersion.RETS_1_5)
	    {
		version = "1.5";
	    }

	    System.out.println("RETS Version: " + version);

	    LogoutResponse logout = session.Logout();

	    System.out.println("Billing info: " + logout.GetBillingInfo());
	    System.out.println("Logout Message:  " + logout.GetLogoutMessage());
	    System.out.println("Connect time: " + logout.GetConnectTime());
	}
	finally
	{
	    if (logger != null)
	        logger.finalize();
	}
    }
}

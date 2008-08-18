using System;
using System.IO;
using librets;

public class Logging
{
    static void Main(string[] args)
    {
	/*
	 * Logging may be performed two different ways. The first is legacy
	 * and uses a logging delegate. This can give more control to the user
	 * and remains to support older .NET implementations that already implemented
	 * logging. The second is simply makes use of the existing logging classes
	 * within libRETS and only requires specifying the log file name.
 	 */
        RetsSession session = null;
        TextWriter logWriter = null;

	/*
	 * Legacy method:
	 */
        try
        {
            session = new RetsSession(
                "http://demo.crt.realtors.org:6103/rets/login");
            if (args.Length == 1)
                logWriter = new StreamWriter(args[0]);
            else
                logWriter = TextWriter.Null;
            
            session.LoggerDelegate =
               TextWriterLogger.CreateDelegate(logWriter);
            if (!session.Login("Joe", "Schmoe"))
            {
                Console.WriteLine("Invalid login");
                Environment.Exit(1);
            }
            
            Console.WriteLine(".Net version: " + System.Environment.Version);

            LoginResponse login = session.GetLoginResponse();
            Console.WriteLine("Member name: " + login.GetMemberName());
            
            CapabilityUrls capabilityUrls = session.GetCapabilityUrls();
            Console.WriteLine("Search URL: " + capabilityUrls.GetSearchUrl());

            LogoutResponse logout = session.Logout();
            Console.WriteLine("Billing info: " + logout.GetBillingInfo());
            Console.WriteLine("Logout message: " + logout.GetLogoutMessage());
            Console.WriteLine("Connect time: " + logout.GetConnectTime());
        }
        finally
        {
            // Be sure to dispose RetsSession when finished, otherwise the
            // TextWriter Dispose() method may be called prior to RetsSession.
            if (session != null)
                session.Dispose();
            logWriter.Close();
        }

	/*
	 * Preferred method.
	 */
        session = null;
        try
        {
            session = new RetsSession(
                "http://demo.crt.realtors.org:6103/rets/login");

	    if (args.Length == 1)
	        session.SetHttpLogName(args[0] + ".1");
            if (!session.Login("Joe", "Schmoe"))
            {
                Console.WriteLine("Invalid login");
                Environment.Exit(1);
            }
            
            Console.WriteLine(".Net version: " + System.Environment.Version);

            LoginResponse login = session.GetLoginResponse();
            Console.WriteLine("Member name: " + login.GetMemberName());
            
            CapabilityUrls capabilityUrls = session.GetCapabilityUrls();
            Console.WriteLine("Search URL: " + capabilityUrls.GetSearchUrl());

            LogoutResponse logout = session.Logout();
            Console.WriteLine("Billing info: " + logout.GetBillingInfo());
            Console.WriteLine("Logout message: " + logout.GetLogoutMessage());
            Console.WriteLine("Connect time: " + logout.GetConnectTime());
        }
        finally
        {
            // Be sure to dispose RetsSession when finished, otherwise the
            // TextWriter Dispose() method may be called prior to RetsSession.
            if (session != null)
                session.Dispose();
        }
    }
}

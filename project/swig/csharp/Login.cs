using System;
using System.IO;
using librets;

public class Login
{
    static void Main(string[] args)
    {
        RetsSession session = null;
        try
        {
            session = new RetsSession(
                "http://demo.crt.realtors.org:6103/rets/login");
	    if (args.Length == 1)
	        session.SetHttpLogName(args[0]);
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

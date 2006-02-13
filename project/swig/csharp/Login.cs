using System;
using System.IO;
using librets;

public class Search
{
    static void Main(string[] args)
    {
        TextWriter logWriter;
        if (args.Length == 1)
            logWriter = new StreamWriter(args[0]);
        else
            logWriter = TextWriter.Null;
            
        try
        {
            RetsSession session = new RetsSession(
                "http://demo.crt.realtors.org:6103/rets/login");
            session.LoggerDelegate =
                TextWriterLogger.CreateDelegate(logWriter);
            if (!session.Login("Joe", "Schmoe"))
            {
                Console.WriteLine("Invalid login");
                Environment.Exit(1);
            }
            LogoutResponse logout = session.Logout();
            Console.WriteLine("Billing info: " + logout.GetBillingInfo());
            Console.WriteLine("Logout message: " + logout.GetLogoutMessage());
            Console.WriteLine("Connect time: " + logout.GetConnectTime());
        }
        finally
        {
            logWriter.Close();
        }
    }
}

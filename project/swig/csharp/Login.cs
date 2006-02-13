using System;
using System.IO;
using librets;

public class Search
{
    private static TextWriter mLogWriter;
    
    public static void ConsoleLogger(RetsHttpLogger.Type type, byte[] data)
    {
        mLogWriter.Write(System.Text.Encoding.UTF8.GetString(data));
        mLogWriter.Flush();
    }
    
    static void Main(string[] args)
    {
        if (args.Length == 1)
            mLogWriter = new StreamWriter(args[0]);
        else
            mLogWriter = TextWriter.Null;
            
        try
        {
            RetsSession session = new RetsSession(
                "http://demo.crt.realtors.org:6103/rets/login");
            session.LoggerDelegate =
                new RetsHttpLogger.Delegate(Search.ConsoleLogger);
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
            mLogWriter.Close();
        }
    }
}

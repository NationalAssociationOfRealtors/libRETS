using System;
using System.Collections;
using librets;

public class Search
{
    public static void MyLogger(RetsHttpLogger.Type type, IntPtr data, int length)
    {
        byte[] bytes = new byte[length];
        System.Runtime.InteropServices.Marshal.Copy(data, bytes, 0, length);
        Console.WriteLine("C# Logger: " + System.Text.Encoding.UTF8.GetString(bytes));
    }
    
    static void Main(string[] args)
    {
        RetsHttpLoggerBridge loggerBridge = new RetsHttpLoggerBridge(
            new RetsHttpLoggerBridge.RetsHttpLoggerDelegate(Search.MyLogger));
        
        RetsSession session = new RetsSession(
            "http://demo.crt.realtors.org:6103/rets/login");
        session.SetHttpLogger(loggerBridge);
        if (!session.Login("Joe", "Schmoe"))
        {
            Console.WriteLine("Invalid login");
            Environment.Exit(1);
        }
        LogoutResponse logout = session.Logout();
        Console.WriteLine("Billing info: " + logout.GetBillingInfo());
        Console.WriteLine("Logout message: " + logout.GetLogoutMessage());
        Console.WriteLine("Connect time: " + logout.GetConnectTime());
        // session.SetHttpLogger(null);
    }
}

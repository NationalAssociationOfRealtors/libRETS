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
	RetsHttpLoggerBridge loggerBridge;
#if true
    {
	RetsHttpLoggerBridge.RetsHttpLoggerDelegate myDelegate =
            new RetsHttpLoggerBridge.RetsHttpLoggerDelegate(Search.MyLogger);
        loggerBridge = new RetsHttpLoggerBridge(myDelegate);
	// The following line causes a crash, if executed, by allowing
	// the delegate to be garbage collected.
	// myDelegate = null;
    }
#else
        loggerBridge = new RetsHttpLoggerBridge(new RetsHttpLoggerBridge.RetsHttpLoggerDelegate(Search.MyLogger));
#endif
	GC.Collect();
        
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
	// GC.KeepAlive(loggerBridge);
	// session.SetHttpLogger(null);
    }
}

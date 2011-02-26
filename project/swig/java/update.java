import java.lang.*;
import librets.*;
import java.io.*;

public class update 
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

        try
        {
            System.out.println("Action: " + session.GetAction());
            /*
             * Create an update request.
             */
            UpdateRequest updateRequest = session.CreateUpdateRequest("Property", "RES");
            updateRequest.SetDelimiter("|");
            updateRequest.SetValidateFlag(2);
            updateRequest.SetUpdateType("Change");
            updateRequest.SetField("CloseDate", "2009-08-20T00:00:00");
            updateRequest.SetField("ListingID", "LN000005");

            /*
             * Perform the update.
             */
            UpdateResponse results = session.Update(updateRequest);

            StringVector columns = null;
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
             * See if there are errors. If so, show them.
             */
            boolean seen = false;
            while (results.HasNextError())
            {
                if (!seen)
                {
                    seen = true;
                    System.out.println("Errors:");
                }

                System.out.println(results.GetErrorFieldName() + 
                    ", Error: " +
                    results.GetErrorNumber() +
                    " at offset " +
                    results.GetErrorOffset() +
                    ", Message: " +
                    results.GetErrorText());
            }

            /*
             * See if there are warnings. If so, show them.
             */

            seen = false;
            while (results.HasNextWarning())
            {
                if (!seen)
                {
                    seen = true;
                    System.out.println("Warnings:");
                }

                System.out.println(results.GetWarningFieldName() + 
                    ", Error: " +
                    results.GetWarningNumber() +
                    " at offset " +
                    results.GetWarningOffset() +
                    ", Message: " +
                    results.GetWarningText() +
                    ", Response Required: " +
                    results.GetWarningResponseRequired());
            }
        }
        catch (Exception e)
        {
            System.out.println(e);
        }

        LogoutResponse logout = session.Logout();

        System.out.println("Billing info: " + logout.GetBillingInfo());
        System.out.println("Logout Message:  " + logout.GetLogoutMessage());
        System.out.println("Connect time: " + logout.GetConnectTime());
    }
}

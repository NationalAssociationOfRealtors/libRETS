using System;
using System.Collections;
using System.IO;
using System.Collections.Specialized;
using librets;

public class Update
{
    static void Main(string[] args)
    {
        Options options  = new Options();

        if (!options.Parse(args))
            Environment.Exit(1);

        RetsSession session = options.SessionFactory();

        try 
        {

            if (!session.Login(options.user_name, options.user_password))
            {
                Console.WriteLine("Invalid login");
                Environment.Exit(1);
            }

            /*
             * Create an update request.
             */
            UpdateRequest updateRequest = session.CreateUpdateRequest(
                                                            options.update_resource,
                                                            options.update_class);
            updateRequest.SetDelimiter("|");
            updateRequest.SetValidateFlag(options.validation_code);
            updateRequest.SetUpdateType(options.update_type);

            /*
             * Pull apart the Record.
             */
            string[] lines = options.update_record.Split(",".ToCharArray());
            /*
             * We should now have key/value pairs in lines.
             * Process them.
             */
            foreach (string line in lines)
            {
                string[] keyValue = line.Split("=".ToCharArray());
                string key = keyValue[0].Trim();
                string value = keyValue[1].Trim();
                updateRequest.SetField(key, value);
            }

            /*
             * Perform the update.
             */
            UpdateResponse results = session.Update(updateRequest);

            IEnumerable columns = null;
            while (results.HasNext())
            {
                if (columns == null)
                {
                    columns = results.GetColumns();
                }
                foreach (string column in columns)
                {
                    Console.WriteLine(column + ": " + results.GetString(column));
                }
                Console.WriteLine();
            }

            /*
             * See if there are errors. If so, show them.
             */
            bool seen = false;
            while (results.HasNextError())
            {
                if (!seen)
                {
                    seen = true;
                    Console.WriteLine("Errors:");
                }

                Console.WriteLine(results.GetErrorFieldName() + 
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
                    Console.WriteLine("Warnings:");
                }

                Console.WriteLine(results.GetWarningFieldName() + 
                    ", Error: " +
                    results.GetWarningNumber() +
                    " at offset " +
                    results.GetWarningOffset() +
                    ", Message: " +
                    results.GetWarningText() +
                    ", Response Required: " +
                    results.GetWarningResponseRequired());
            }

            LogoutResponse logout = session.Logout();
            Console.WriteLine("Billing info: " + logout.GetBillingInfo());
            Console.WriteLine("Logout message: " + logout.GetLogoutMessage());
            Console.WriteLine("Connect time: " + logout.GetConnectTime());
        } 
        catch (Exception e)
        {
            Console.WriteLine("RetsException: " + e);
            Environment.Exit(1);
        }
    }
}

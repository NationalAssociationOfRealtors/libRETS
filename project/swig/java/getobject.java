import java.lang.*;
import java.io.*;
import librets.*;

public class getobject 
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

        GetObjectRequest objectRequest = new GetObjectRequest("Property", "Photo");

        objectRequest.AddAllObjects("LN000001");
        GetObjectResponse response = session.GetObject(objectRequest);
        
        ObjectDescriptor objectDescriptor = response.NextObject();

        while (objectDescriptor != null)
        {
            String object_key   = objectDescriptor.GetObjectKey();
            int object_id       = objectDescriptor.GetObjectId();
            String content_type = objectDescriptor.GetContentType();
            String description  = objectDescriptor.GetDescription();
	    String location     = objectDescriptor.GetLocationUrl();
	    int    reply_code   = objectDescriptor.GetRetsReplyCode();
        
            System.out.print(object_key + " object #" + object_id);

            if (description.length() > 0)
                System.out.print(", description: " + description);
	    if (location.length() > 0)
	        System.out.print(", location: " + location);
	    if (reply_code != 0)
	        System.out.print(", ***** " + reply_code + ": " +
		                 objectDescriptor.GetRetsReplyText());
            System.out.println();

            String suffix = "jpg";
            if (content_type.equals("image/gif"))
                suffix = "gif";
            if (content_type.equals("text/xml"))
                suffix = "xml";

            String file_name = object_key + "-" + object_id + "." + suffix;

	    /*
	     * Only save the object if the reply code is zero and we are not
	     * using location=1.
	     */
	    if (reply_code == 0 && location.length() == 0)
            try
            {
                FileOutputStream outfile = new FileOutputStream(file_name);

                byte [] object_data = objectDescriptor.GetDataAsBytes();

                outfile.write(object_data);
                outfile.close();
            }
            catch (IOException e) {}

            objectDescriptor = response.NextObject();
        }

        LogoutResponse logout = session.Logout();

        System.out.println("Billing info: " + logout.GetBillingInfo());
        System.out.println("Logout Message:  " + logout.GetLogoutMessage());
        System.out.println("Connect time: " + logout.GetConnectTime());
    }
}

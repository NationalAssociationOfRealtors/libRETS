using System;
using System.Collections;
using System.IO;
using librets;

public class Search
{
    static void Main(string[] args)
    {
        RetsSession session = new RetsSession(
            "http://demo.crt.realtors.org:6103/rets/login");
        if (!session.Login("Joe", "Schmoe"))
        {
            Console.WriteLine("Invalid login");
            Environment.Exit(1);
        }

        GetObjectRequest request = new GetObjectRequest("Property", "Photo");
        request.AddAllObjects("LN000001");
        
        GetObjectResponse response = session.GetObject(request);
        ObjectDescriptor objectDescriptor;
        while ((objectDescriptor = response.NextObject()) != null)
        {
            string objectKey = objectDescriptor.GetObjectKey();
            int objectId = objectDescriptor.GetObjectId();
            string contentType = objectDescriptor.GetContentType();
            string description = objectDescriptor.GetDescription();
            
            Console.Write(objectKey + " object #" + objectId);
            if (description.Length != 0)
                Console.Write(", desription: " + description);
            Console.WriteLine();
            
            /* This doesn't work yet...
            string data = objectDescriptor.GetStringData();
            Console.WriteLine("Data length: " + data.Length);
            string suffix = "jpg";
            string outputFileName = objectKey + "-" + objectId + "." + suffix;
            Stream outputStream = File.OpenWrite(outputFileName);
            BinaryWriter w = new BinaryWriter(outputStream);
            w.Write(data);
            w.Close();
            outputStream.Close();
            */
        }

        session.Logout();
    }
}

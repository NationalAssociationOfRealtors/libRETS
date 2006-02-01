using System;
using System.Collections;
using System.IO;
using librets;

public class Search
{
    static void Main(string[] args)
    {
        bool useStream = true;
        if ((args.Length == 1) && args[0].Equals("bytes"))
        {
            Console.WriteLine("Using GetDataAsBytes()");
            useStream = false;
        }

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
        foreach(ObjectDescriptor objectDescriptor in response)
        {
            string objectKey = objectDescriptor.GetObjectKey();
            int objectId = objectDescriptor.GetObjectId();
            string contentType = objectDescriptor.GetContentType();
            string description = objectDescriptor.GetDescription();
            
            Console.Write(objectKey + " object #" + objectId);
            if (description.Length != 0)
                Console.Write(", desription: " + description);
            Console.WriteLine();
            
            Hashtable extensions = new Hashtable();
            extensions["image/jpeg"] = "jpg";
            extensions["image/gif"] = "gif";
            
            string extension = (string) extensions[contentType];
            string outputFileName = objectKey + "-" + objectId + "." +
                extension;
            Stream outputStream = File.OpenWrite(outputFileName);
            if (useStream)
            {
                const int BUFFER_SIZE =  1024;
                Stream stream = objectDescriptor.GetDataStream();
                byte[] buffer = new Byte[BUFFER_SIZE];
                int bytesRead;
                while ((bytesRead = stream.Read(buffer, 0, BUFFER_SIZE)) > 0)
                {
                    outputStream.Write(buffer, 0, bytesRead);
                }
            }
            else
            {
                byte[] data = objectDescriptor.GetDataAsBytes();
                BinaryWriter w = new BinaryWriter(outputStream);
                w.Write(data);
                w.Close();
            }

            outputStream.Close();
        }

        session.Logout();
    }
}

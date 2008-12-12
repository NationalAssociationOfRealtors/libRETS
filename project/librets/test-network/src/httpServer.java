
//***************************************
// HTTP Server
//***************************************


import java.net.*;
import java.io.*;
import java.util.*;
import java.lang.*;
import javax.net.ServerSocketFactory;
import javax.net.ssl.SSLServerSocketFactory;
import javax.net.ssl.SSLServerSocket;


public class httpServer
{
    public static void main(String args[]) 
    {
        optionsParser options   = new optionsParser(args);

        int port                = options.getPort();
        boolean verbose         = options.getVerbose();

	ServerSocketFactory socketFactory;
        ServerSocket server_socket;

        try {
           
	   if (options.getSSL())
	   	socketFactory	= SSLServerSocketFactory.getDefault();
	   else
	   	socketFactory	= ServerSocketFactory.getDefault();

           //print out the port number for user
            server_socket = socketFactory.createServerSocket(port);

            System.out.println("httpServer running on port " +
               server_socket.getLocalPort());
           
            // server infinite loop
            while(true) 
            {
                Socket socket = server_socket.accept();
                System.out.println("New connection accepted " +
                   socket.getInetAddress() +
                       ":" + socket.getPort());

                // Construct handler to process the HTTP request message.
                try 
                {
                    httpRequestHandler request = new httpRequestHandler(socket, options);
                    // Create a new thread to process the request.
                    Thread thread = new Thread(request);
               
                    // Start the thread.
                    thread.start();
                }
                catch(Exception e) {
                    System.out.println(e);
                }
            }
        }

        catch (IOException e) {
            System.out.println(e);
        }
    }
}

class httpRequestHandler implements Runnable
{
    final static String CRLF = "\r\n";
    Socket socket;
    InputStream input;
    OutputStream output;
    BufferedReader br;
    optionsParser options;
    Properties properties = new Properties();
    String resourcePath;
    boolean verbose;
    boolean delay;
    int delay_interval = 100;

    // Constructor
    public httpRequestHandler(Socket socket, optionsParser options) throws Exception
    {
        FileInputStream fis = new FileInputStream(options.getResourcePath() + "/" + options.getProperties());
        properties.load(fis);
        this.socket = socket;
        this.input = socket.getInputStream();
        this.output = socket.getOutputStream();
        this.br = new BufferedReader(new InputStreamReader(socket.getInputStream()));
        this.options = options;
        this.resourcePath  = options.getResourcePath();
        this.delay = options.getDelay();
        if (this.delay)
        {
            this.delay_interval = options.getDelayInterval();
        }
        this.verbose = options.getVerbose();
    }
   
    // Implement the run() method of the Runnable interface.
    public void run()
    {
        try 
        {
            processRequest();
        }
        catch(Exception e) {
            System.out.println("Exception in run()");
            System.out.println(e);
        }
    }
   
    private void processRequest() throws Exception
    {
        while(true) 
        {
            String headerLine = br.readLine();
            if (verbose)
            {
                System.out.println("Header: \"" + headerLine + "\"");
            }

            // See if we're done.
            if(headerLine.equals(CRLF) || headerLine.equals("")) break; 
           
            // Determine if we are doing a GET or POST.

            StringTokenizer s = new StringTokenizer(headerLine);
            String temp = s.nextToken();
           
            if (temp.equals("POST"))
            {
                int contentLength = 0;
                if (verbose)
                {
                    System.out.println("Processing POST");
                }

                // The "url" is going to be a directory that will contain the script
                // to play back.
                String thePath = s.nextToken();

                // Prime the pump to read the balance of the header information.
                String theLine = br.readLine();

                while (theLine != null)
                {
                    if (verbose)
                    {
                        System.out.println("Header : \"" + theLine + "\"");
                    }

                    if (theLine.equals(CRLF) || theLine.equals(""))
                    {
                        // End of headers. We should have already seen the content
                        // length header. Read the content.
                        char [] ca = new char [contentLength];
                        int len = br.read(ca, 0, contentLength);
                        theLine = new String(ca);
                        if (verbose)
                        {
                            System.out.println("Content: \"" + theLine + "\"");
                        }

                        //
                        // The content will refer to a key in the properties file 
                        // related to the "url" as in the GET option. This is the
                        // key will be translated to the file to be played back.
                        // For example:
                        //   POST /rets/getMetadata
                        //   content: Format=COMPACT&ID=0&Type=METADATA-SYSTEM
                        // would look for the key "/rets/getMetadata/Format=COMPACT&ID=0&Type=METADATA-SYSTEM"
                        // in the files.properties file. The value associated with that key will be the
                        // file to be played back.
                        // 
                        // Note that the special tokens in the key must be escaped in the properties
                        // file. So, the above key=value would look like:
                        // /rets/getMetadata\?Format\=COMPACT\&ID\=0\&Type\=METADATA-SYSTEM = metadata-system
                        //
                        String fileName;
                        fileName = resourcePath + "/" + properties.getProperty(thePath + "?" + theLine);
                        if (verbose)
                        {
                            System.out.println("Looking for playback file: " + fileName);
                        }

                        // Open the requested file.
                        FileInputStream fis = null ;
                        boolean fileExists = true ;
                        try
                        {
                            fis = new FileInputStream( fileName ) ;
                        }
                        catch ( FileNotFoundException e )
                        {
                            fileExists = false ;
                        }

                        if (fileExists)
                        {
                            sendBytes(fis, output) ;
                            fis.close();
                        }
                        else
                        {
                            String statusLine = "HTTP/1.0 404 Not Found" + CRLF ;
                            String contentTypeLine = "text/html" ;
                            String entityBody = "<HTML>" +
                                    "<HEAD><TITLE>404 Not Found</TITLE></HEAD>" +
                                    "<BODY>404 Not Found"
                                    + "<br>usage:http://localhost:6103/"
                                    + fileName + "</BODY></HTML>" ;
                            // Send the status line.
                            output.write(statusLine.getBytes());

                            // Send the content type line.
                            output.write(contentTypeLine.getBytes());

                            // Send a blank line to indicate the end of the header lines.
                            output.write(CRLF.getBytes());

                            output.write(entityBody.getBytes());
                        }
                        // We're done with this cycle.
                        break;
                    }
                    else
                    {
                        // Process the header.
                        s = new StringTokenizer(theLine);
                        temp = s.nextToken();
                        if (temp.equals("Content-Length:"))
                        {
                            temp = s.nextToken();
                            
                            contentLength = Integer.parseInt(temp);
                        }
                        // Read the next header/content line.
                        theLine = br.readLine();
                    }
                }
                break;
            }

            if(temp.equals("GET")) 
            {
                String fileName = s.nextToken();

                fileName = resourcePath + "/" + properties.getProperty(fileName);
                if (verbose)
                {
                    System.out.println("Looking for playback file: " + fileName);
                }

                // Open the requested file.
                FileInputStream fis = null ;
                boolean fileExists = true ;
                try
                {
                    fis = new FileInputStream( fileName ) ;
                }
                catch ( FileNotFoundException e )
                {
                    fileExists = false ;
                }

                // Send the entity body.
                if (fileExists)
                {
                    sendBytes(fis, output) ;
                    fis.close();
                }
                else
                {
                    String statusLine = "HTTP/1.0 404 Not Found" + CRLF ;
                    String contentTypeLine = "text/html" ;
                    String entityBody = "<HTML>" +
                            "<HEAD><TITLE>404 Not Found</TITLE></HEAD>" +
                            "<BODY>404 Not Found"
                            + "<br>usage:http://localhost:6103/"
                            + fileName + "</BODY></HTML>" ;
                    // Send the status line.
                    output.write(statusLine.getBytes());

                    // Send the content type line.
                    output.write(contentTypeLine.getBytes());

                    // Send a blank line to indicate the end of the header lines.
                    output.write(CRLF.getBytes());

                    output.write(entityBody.getBytes());
                }

            }
        }

        try 
        {
            output.close();
            br.close();
            socket.close();
        }
        catch(Exception e) {}
    }
   
    private void sendBytes(FileInputStream fis, OutputStream os) throws Exception
    {
        // Construct a 1K buffer to hold bytes on their way to the socket.
        byte[] buffer = new byte[1024] ;
        int bytes = 0 ;

        // Copy requested file into the socket's output stream.
        while ((bytes = fis.read(buffer)) != -1 )
        {
            // Need to add an artificial delay to slow things down in order to exercise the streaming interface.
            // If we don't do this, the entire file could get sent as a single packet.

            if (delay)
            {
                Thread.sleep(delay_interval);
            }
            {
                String s = new String(buffer,0,bytes);
                if (verbose)
                    System.out.println ("Sending: \"" + s + "\"");
            }
            os.write(buffer, 0, bytes);
        }
    }
}

class optionsParser
{
    int mPort = 6103;
    String mProperties = "files.properties";
    String mResourcePath = "./resources";
    boolean mDelay = false;
    int mDelayInterval = 100;
    boolean mSSL = false;
    boolean mVerbose = false;

    public optionsParser(String args[]) 
    {
        try
        {
            for (int i = 0; i < args.length; i++)
            {
                String key = "";
                String value = "";
                StringTokenizer s = new StringTokenizer(args[i],"=");
                if (s.hasMoreTokens())
                {
                    // There really better be at least one token!
                    // This should be an option keyword.
                    key = s.nextToken().toLowerCase();
                    if (s.hasMoreTokens())
                    {
                        value = s.nextToken();
                    }
                }
                else
                {
                    throw new Exception();
                }

                if (key.equals("--delay"))
                {
                    mDelay = true;
                    try
                    {
                        mDelayInterval = Integer.parseInt(value);
                    }
                    catch (Exception e)
                    {
                        mDelayInterval = 100;
                    }
                }
                else
                if (key.equals("--resource"))
                {
                    mResourcePath = value;
                }
                else
                if (key.equals("--port"))
                {
                    mPort = Integer.parseInt(value);
                }
                else
                if (key.equals("--properties"))
                {
                    mProperties = value;
                }
                else
                if (key.equals("--ssl"))
                {
                    mSSL = true;
                }
                else
                if (key.equals("--verbose"))
                {
                    mVerbose = true;
                }
                else
                {
                   throw new Exception();
                }
                if(mVerbose)
                {
                    System.out.println(key + ": " + value);
                }
            }
        }
        catch (Exception e)
        {
            Usage();
            System.exit(1);
        }
    }

    public boolean getDelay()
    {
        return mDelay;
    }

    public int getDelayInterval()
    {
        return mDelayInterval;
    }

    public String getProperties()
    {
        return mProperties;
    }

    public String getResourcePath()
    {
        return mResourcePath;
    }

    public int getPort()
    {
        return mPort;
    }

    public boolean getSSL()
    {
        return mSSL;
    }

    public boolean getVerbose()
    {
        return mVerbose;
    }

    public void Usage()
    {
        System.out.println("--resource=<path>    Path to resource directory.");
        System.out.println("--port=<port>        Port number to listen on. Default 6103.");
        System.out.println("--delay[=<ms>]       Add artificial delay when responding to POST. Default 100ms.");
        System.out.println("--properties=<file>  Filename mapping file. Default files.properties.");
        System.out.println("--ssl                Set ssl mode.");
        System.out.println("--verbose            Set verbose option.");
    }
}
 

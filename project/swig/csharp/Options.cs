using System;
using System.Collections;
using System.IO;
using System.Collections.Specialized;
using librets;

/**
 * Handle all of the user customizable options and set their default values.
 * This includes a parser method that is "brute force" and does minimal
 * validity checking.
 */
public class Options
{
    private string mBroker                  = "";
    private SearchRequest.CountType mCount  = SearchRequest.CountType.RECORD_COUNT_AND_RESULTS;
    private EncodingType mEncoding          = EncodingType.RETS_XML_DEFAULT_ENCODING;
    private string mHttpLog                 = "";
    private int mLimit                      = -1;
    private bool mLogEverything             = false;
    private TextWriter mLogWriter           = TextWriter.Null;
    private string mMetadataTimestamp       = "";
    private int mOffset                     = 0;
    private string mPass                    = "Schmoe";
    private string mQuery                   = "(ListPrice=300000-)";
    private RetsSession mRetsSession        = null;
    private RetsVersion mRetsVersion        = RetsVersion.RETS_1_5;
    private string mSearchType              = "Property";
    private string mSelect                  = "ListingID,ListPrice,Beds,City";
    private string mSearchClass             = "ResidentialProperty";
    private bool mStandardNames             = true;
    private string mUA                      = "librets/" + RetsSession.GetLibraryVersion();
    private UserAgentAuthType mUAAuth       = UserAgentAuthType.USER_AGENT_AUTH_RETS_1_7;
    private string mUAPass                  = "";
    private string mUrl                     = "http://demo.crt.realtors.org:6103/rets/login";
    private string mUser                    = "Joe";

    /**
     * The broker code.
     */
    public string broker
    {
        get { return mBroker; }
        set { mBroker = value; }
    }

    /**
     * The Search COUNT type.
     */
    public SearchRequest.CountType count
    {
        get { return mCount; }
        set { mCount = value; }
    }

    /**
     * The default encoding for the XML parser.
     */
    public EncodingType encoding
    {
        get { return mEncoding; }
        set {mEncoding = value; }
    }

    /**
     * The default encoding for the XML parser expressed
     * as a string. Valid values are "US-ASCII" or "ISO".
     */
    public string encoding_string
    {
        get
        {
            switch (mEncoding)
            {
                case EncodingType.RETS_XML_DEFAULT_ENCODING: return "US-ASCII";
                case EncodingType.RETS_XML_ISO_ENCODING: return "ISO";
            }

            mEncoding = EncodingType.RETS_XML_DEFAULT_ENCODING;
            return "US-ASCII";
        }
        set
        {
            mEncoding = EncodingType.RETS_XML_DEFAULT_ENCODING;
            if (value.ToLower() == "us-ascii")
                mEncoding = EncodingType.RETS_XML_DEFAULT_ENCODING;
            else
            if (value.ToLower() == "iso")
                mEncoding = EncodingType.RETS_XML_ISO_ENCODING;
        }
    }

    /**
     * The search COUNT field expressed as a string. Valid values are
     * "yes", "no", "count-only".
     */
    public string count_string
    {
        get
        {
            switch (mCount)
            {
                case SearchRequest.CountType.NO_RECORD_COUNT: return "no";
                case SearchRequest.CountType.RECORD_COUNT_AND_RESULTS: return "yes";
                case SearchRequest.CountType.RECORD_COUNT_ONLY: return "count-only";
            }
            return "yes";
        }

        set
        {
            mCount = SearchRequest.CountType.RECORD_COUNT_AND_RESULTS;

            if (value.ToLower() == "no")
                mCount = SearchRequest.CountType.NO_RECORD_COUNT;
            else
            if (value.ToLower() == "yes")
                mCount = SearchRequest.CountType.RECORD_COUNT_AND_RESULTS;
            else
            if (value.ToLower() == "count-only")
                mCount = SearchRequest.CountType.RECORD_COUNT_ONLY;
        }
    }

    /**
     * The name of the log file to which http transactions will be written by
     * libCURL.
     */
    public string http_log
    {
        get { return mHttpLog; }
        set { mHttpLog = value; }
    }

    /**
     * The search LIMIT.
     */
    public int limit
    {
        get { return mLimit; }
        set {mLimit = value; }
    }

    /**
     * A flag to indicate whether or not GetObject activity is logged.
     */
    public bool log_everything
    {
        get { return mLogEverything; }
        set { mLogEverything = value; }
    }

    /**
     * The RETS server login URL.
     */
    public string login_url
    {
        get { return mUrl; }
        set { mUrl = value; }
    }

    /**
     * A properly formatted timestamp containing the last metadata timetsamp.
     */
    public string metadata_timestamp
    {
        get {return mMetadataTimestamp; }
        set { mMetadataTimestamp = value; }
    }

    /**
     * The search OFFSET value.
     */
    public int offset
    {
        get {return mOffset; }
        set {mOffset = value; }
    }

    /**
     * The search QUERY string.
     */
    public string query
    {
        get {return mQuery; }
        set {mQuery = value; }
    }

    /**
     * The RETS version to use. Valid values are "1.0", "1.5" or "1.7".
     */
    public string RETS_version
    {
        get 
        { 
            switch (mRetsVersion)
            {
                case RetsVersion.RETS_1_0:  return "1.0";
                case RetsVersion.RETS_1_5:  return "1.5";
                case RetsVersion.RETS_1_7:  return "1.7";
            }
            return "1.0"; 
        }
        set 
        { 
            mRetsVersion = RetsVersion.RETS_1_5;
            if (value == "1.0")
                mRetsVersion = RetsVersion.RETS_1_0;

            if (value == "1.5")
                mRetsVersion = RetsVersion.RETS_1_5;

            if (value == "1.7")
                mRetsVersion = RetsVersion.RETS_1_7;
        }
    }

    /**
     * The search CLASS.
     */
    public string search_class
    {
        get {return mSearchClass; }
        set {mSearchClass = value; }
    }

    /**
     * The search RESOURCE.
     */
    public string search_type
    {
        get {return mSearchType; }
        set {mSearchType = value; }
    }

    /**
     * The search SELECT.
     */
    public string select
    {
        get {return mSelect; }
        set {mSelect = value; }
    }

    /**
     * Controls whether or not to use system names or
     * standard names.
     */
    public bool standard_names
    {
        get {return mStandardNames; }
        set {mStandardNames = value; }
    }

    /**
     * The User-Agent password.
     */
    public string ua_password
    {
        get { return mUAPass; }
        set { mUAPass = value; }
    }

    /**
     * The User-Agent.
     */
    public string user_agent
    {
        get { return mUA; }
        set { mUA = value; }
    }

    /**
     * The User name for logging in.
     */
    public string user_name
    {
        get { return mUser; }
        set { mUser = value; }
    }

    /**
     * The user password.
     */
    public string user_password
    {
        get { return mPass; }
        set { mPass = value; }
    }

    /**
     * A very simple parsing method to pull apart the arguments.
     */
    public bool Parse( string[] args)
    {
        string key      = "";
        bool ret_val    = true;
        string value    = "";
        for (int i = 0; i < args.Length; i += 2)
        {
            try
            {
                key     = args[i].ToString().ToLower();
                value   = args[i + 1].ToString();
            }
            catch
            {
                ret_val = false;
                break;
            }
            
            switch (key)
            {
                case "--broker-code":
                            broker              = value;
                            break;
                case "--class":
                            search_class        = value;
                            break;
                case "--count":
                            count_string        = value;
                            break;
                case "--default-encoding":
                            encoding_string     = value;
                            break;
                case "--http-log":
                            try
                            {
                                mLogWriter      = new StreamWriter(value);
                                http_log        = value;
                                mLogEverything  = false;
                            }
                            catch
                            {
                                mLogWriter      = TextWriter.Null;
                                Console.WriteLine("Expected valid file name for --http-log: " +
                                        value + " is invalid.");
                                ret_val         = false;
                            }
                            break;
                case "--http-log-everything":
                            try
                            {
                                mLogWriter      = new StreamWriter(value);
                                http_log        = value;
                                mLogEverything  = true;
                            }
                            catch
                            {
                                mLogWriter      = TextWriter.Null;
                                Console.WriteLine("Expected valid file name for --http-log: " +
                                        value + " is invalid.");
                                ret_val         = false;
                            }
                            break;
                case "--limit":
                            limit               = int.Parse(value);
                            break;
                case "--metadata-timestamp":
                            metadata_timestamp  = value;
                            break;
                case "--offset":
                            offset              = int.Parse(value);
                            break;
                case "--password":
                            user_password       = value;
                            break;
                case "--query":
                            query               = value;
                            break;
                case "--resource":
                            search_type         = value;
                            break;
                case "--rets_version":
                            RETS_version        = value;
                            break;
                case "--standard-names":
                            if (value.ToLower() == "yes")
                                standard_names        = true;
                            if (value.ToLower() == "no")
                                standard_names  = false;
                            break;
                case "--select":
                            select              = value;
                            break;
                case "--url":
                            login_url           = value;
                            break;
                case "--username":
                            user_name           = value;
                            break;
                case "--user-agen":
                            user_agent          = value;
                            break;
                case "--ua-password":
                            ua_password         = value;
                            break;
                default:
                            ret_val             = false;
                            break;
            }
        }
        if (!ret_val)
            Usage();

        return ret_val;
    }

    public void Usage()
    {
        Console.WriteLine("Available switches:");
        Console.WriteLine("\t--url\t\t\tLogin URL");
        Console.WriteLine("\t--username\t\tUser Name");
        Console.WriteLine("\t--password\t\tUser Password");
        Console.WriteLine("\t--user-agent\t\tUser Agent");
        Console.WriteLine("\t--ua-password\t\tUser Agent Password");
        Console.WriteLine("\t--http-log\t\tHTTP log file - won't log GetObject calls");
        Console.WriteLine("\t--http-log-everything\tHttp log file - log GetObject calls");
        Console.WriteLine("\t--rets_version\t\tRETS Version");
        Console.WriteLine("\t--broker-code\t\tBroker Code");
        Console.WriteLine("\t--metadata-timestamp\tMetadata Timestamp");
        Console.WriteLine("\t--resource\t\tSearch resource");
        Console.WriteLine("\t--class\t\t\tSearch class");
        Console.WriteLine("\t--select\t\tSearch select");
        Console.WriteLine("\t--query\t\t\tSearch query");
        Console.WriteLine("\t--standard-names\tUse standard-names: \"yes\" or \"no\" (default \"no\")");
        Console.WriteLine("\t--limit\t\t\tSet the limit (default NONE)");
        Console.WriteLine("\t--offset\t\tSet the offset (default 0)");
        Console.WriteLine("\t--count\t\t\tSet count type: \"no\", \"yes\", or \"count-only\"");
        Console.WriteLine("\t--default-encoding\t\"US-ASCII\" or \"ISO\"");
    }

    /**
     * Create a RetsSession and populate it with all the known options.
     */
    public RetsSession SessionFactory()
    {
        if (mRetsSession != null)
        {
            mRetsSession.Cleanup();
            mRetsSession = null;
        }
        mRetsSession = new RetsSession(mUrl);

        mRetsSession.LoggerDelegate        = TextWriterLogger.CreateDelegate(mLogWriter);

        mRetsSession.SetDefaultEncoding(mEncoding);
        mRetsSession.SetLogEverything(mLogEverything);
        mRetsSession.SetRetsVersion(mRetsVersion);
        mRetsSession.SetUserAgent(mUA);
        mRetsSession.SetUserAgentAuthType(mUAAuth);
        if (mUAPass.Length > 0)
            mRetsSession.SetUserAgentPassword(mUAPass);

        return mRetsSession;
    }
}
    

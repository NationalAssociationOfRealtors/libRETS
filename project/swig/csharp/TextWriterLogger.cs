using System;
using System.IO;

namespace librets {

public class TextWriterLogger
{
    public TextWriterLogger(TextWriter writer)
    {
        mLastType = RetsHttpLogger.Type.INFORMATIONAL;
        mWriter = writer;
        mDelegate = new RetsHttpLogger.Delegate(this.LogData);
    }
    
    private void LogData(RetsHttpLogger.Type type, byte[] data)
    {
        if ((type == RetsHttpLogger.Type.RECEIVED) &&
            (mLastType != RetsHttpLogger.Type.RECEIVED))
        {
            mWriter.WriteLine("\n<<< Received");
        }
        else if ((type == RetsHttpLogger.Type.SENT) &&
            (mLastType != RetsHttpLogger.Type.SENT))
        {
            mWriter.WriteLine("\n>>> Sent");
        }
        else if (type == RetsHttpLogger.Type.INFORMATIONAL)
        {
            mWriter.Write("* ");
        }
        
        mWriter.Write(System.Text.Encoding.UTF8.GetString(data));
        mWriter.Flush();
        mLastType = type;
    }
    
    public RetsHttpLogger.Delegate LoggerDelegate
    {
        get { return mDelegate; }
    }
    
    public static RetsHttpLogger.Delegate CreateDelegate(TextWriter writer)
    {
        TextWriterLogger logger = new TextWriterLogger(writer);
        return logger.LoggerDelegate;
    }
    
    
    private RetsHttpLogger.Type mLastType;
    private TextWriter mWriter;
    private RetsHttpLogger.Delegate mDelegate;
}

}

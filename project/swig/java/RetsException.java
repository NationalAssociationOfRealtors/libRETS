package librets;

public class RetsException
    extends java.lang.Exception
{
    public RetsException(String message)
    {
        mMessage = message;
        mExtendedMessage = "";
    }

    public RetsException(String message, String extendedMessage)
    {
        mMessage = message;
        mExtendedMessage = extendedMessage;
    }

    public String getMessage()
    {
        return mMessage;
    }

    public String getExtendedMessage()
    {
        return mExtendedMessage;
    }

    private String mMessage;
    private String mExtendedMessage;
}

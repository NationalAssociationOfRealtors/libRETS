package librets;

public class RetsReplyException
    extends RetsException
{
    public RetsReplyException(int replyCode, String meaning)
    {
        super(meaning);
        mReplyCode = replyCode;
    }

    public RetsReplyException(int replyCode, String meaning,
                              String extendedMeaning)
    {
        super(meaning, extendedMeaning);
        mReplyCode = replyCode;
    }

    public int getReplyCode()
    {
        return mReplyCode;
    }

    private int mReplyCode;
}

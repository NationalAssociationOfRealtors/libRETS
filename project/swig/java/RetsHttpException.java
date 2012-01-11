package librets;

public class RetsHttpException
    extends RetsException
{
    public RetsHttpException(int httpResult, String meaning)
    {
        super(meaning);
        mHttpResult = httpResult;
    }

    public int getHttpResult()
    {
        return mHttpResult;
    }

    private int mHttpResult;
}

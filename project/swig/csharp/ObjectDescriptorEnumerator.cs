using System;
using System.Collections;

namespace librets {

public class ObjectDescriptorEnumerator : IEnumerator
{
    public ObjectDescriptorEnumerator(GetObjectResponse response)
    {
        mResponse = response;
        mCurrent = null;
        
    }
    
    public object Current
    {
        get { return mCurrent; }
    }
    
    public bool MoveNext()
    {
        mCurrent = mResponse.NextObject();
        return (mCurrent != null);
    }
    
    public void Reset()
    {
        throw new InvalidOperationException(
            "ObjectDescriptorEnumerators are not resetable");
    }
    
    private GetObjectResponse mResponse;
    private ObjectDescriptor mCurrent;
}

}
package librets;

import librets.*;

public class CppInputStream extends java.io.InputStream
{
    private InputStreamBridge mBridge;

    public CppInputStream(InputStreamBridge bridge)
    {
        mBridge = bridge;
    }

    public void close() throws java.io.IOException
    {
    }

    public int read() throws java.io.IOException
    {
        return mBridge.readByte();
    }

    public int read(byte[] buffer, int offset, int count)
    		throws java.io.IOException
    {
         return mBridge.read(buffer, offset, count);
    }
}

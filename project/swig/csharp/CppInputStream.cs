using System;
using System.IO;
using System.Runtime.InteropServices;

namespace librets {

public class CppInputStream : Stream
{
    public CppInputStream(InputStreamBridge bridge)
    {
        mBridge = bridge;
    }

    public override bool CanRead
    {
        get { return true; }
    }
    
    public override bool CanSeek
    {
        get { return false; }
    }
    
    public override bool CanWrite
    {
        get { return false; }
    }
    
    public override long Length
    {
        get { throw new NotSupportedException("Seeking not supported"); }
    }
    
    public override long Position
    {
        get { throw new NotSupportedException("Seeking not supported"); }
        set { throw new NotSupportedException("Seeking not supported"); }
    }
    
    public override void Flush()
    {
        throw new NotSupportedException("Writing not supported");
    }
    
    public override long Seek(long offset, SeekOrigin origin)
    {
        throw new NotSupportedException("Seeking not supported");
    }
    
    public override void SetLength(long length)
    {
        throw new NotSupportedException("Seeking not supported");
    }
    
    public override void Write(byte[] buffer, int offset, int count)
    {
        throw new NotSupportedException("Writing not supported");
    }
    
    public override int ReadByte()
    {
        return mBridge.readByte();
    }

    public override int Read([In,Out] byte[] buffer, int offset, int count)
    {
         return (int) mBridge.read(buffer, (uint) offset, (uint) count);
    }
    
    private InputStreamBridge mBridge;
}

}

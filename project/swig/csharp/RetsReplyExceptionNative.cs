using System;

namespace librets
{
	public class RetsReplyExceptionNative : RetsExceptionNative
	{
		public RetsReplyExceptionNative(int replyCode, string message, string extendedMessage)
			: base(message, extendedMessage)
		{
			this.replyCode = replyCode;
		}
		private int replyCode;
		public int ReplyCode
		{
			get { return replyCode; }
		}
	}
}
using System;

namespace librets
{
	public class RetsHttpExceptionNative : RetsExceptionNative
	{
		public RetsHttpExceptionNative(int httpResult, string message)
			: base(message, "")
		{
			this.httpResult = httpResult;
		}
		private int httpResult;
		public int HttpResult
		{
			get { return httpResult; }
		}
	}
}

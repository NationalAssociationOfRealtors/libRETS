using System;

namespace librets {
	public class RetsExceptionNative : Exception {
		public RetsExceptionNative(string message, string extendedMessage) : base(message) {
			this.extendedMessage = extendedMessage;
		}
		private string extendedMessage;
		public string ExtendedMessage
		{
			get { return extendedMessage; }
		}
	}
}
#!/usr/bin/env python
import sys

import librets

try:
    session = librets.RetsSession("http://www.dis.com:6103/rets/login")

    log_file = sys.argv[1]
    session.SetHttpLogName(log_file)

    if not session.Login("Joe", "Schmoe"):
        sys.exit("Invalid login")

    logout = session.Logout()
    session.Cleanup()

except librets.RetsException, e:
    print "Caught: " + e.GetMessage()


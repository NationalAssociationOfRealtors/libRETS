#!/usr/bin/env python

import sys
import librets

try:
  session = librets.RetsSession("http://demo.crt.realtors.org:6103/rets/login")

  log_file = sys.argv[1]
  session.SetHttpLogName(log_file)

  if (not session.Login("Joe", "Schmoe")):
      print "Invalid login"
      sys.exit(1)

  
  logout = session.Logout()

  session.Cleanup()

except librets.RetsException,e:
  print "Caught: " + e.GetMessage()


#!/usr/bin/env python

import sys
import librets

# To be able to provide a logger to libRETS, you must have an
# initialize method that calls super and implement a method named
# logHttpData that takes two arguements.
class TestLogger(librets.RetsHttpLogger):
  def __init__(self, filename = None):
    librets.RetsHttpLogger.__init__(self)
    if filename:
      self.file = open(filename, 'wa')
    else:
      self.file = sys.stdout

    self.last_type = self.INFORMATIONAL

  # This example logHttpData mirrors the funtionality of the
  # StreamHttpLogger C++ class that ship with the C++ parts of
  # libRETS.
  def logHttpData(self, type, data):
    if type == self.RECEIVED and self.last_type != self.RECEIVED:
      print >> self.file, "\n<<< Received"
    elif type == self.SENT and self.last_type != self.SENT:
      print >> self.file, "\n>>> Sent"
    elif type == self.INFORMATIONAL:
      print >> self.file, "*",

    print >> self.file, data,
    self.last_type = type
    
try:
  session = librets.RetsSession("http://demo.crt.realtors.org:6103/rets/login")

  log_file = sys.argv[1]
  logger = TestLogger(log_file)
  session.SetHttpLogger(logger)

  if (not session.Login("Joe", "Schmoe")):
      print "Invalid login"
      sys.exit(1)

  
  logout = session.Logout()

  session.Cleanup()

except librets.RetsException,e:
  print "Caught: " + e.GetMessage()


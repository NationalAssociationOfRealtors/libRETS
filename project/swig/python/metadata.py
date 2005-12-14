#!/usr/bin/env python

import librets

try:
  rets = librets.RetsSession("http://demo.crt.realtors.org:6103/rets/login")
  rets.Login("Joe", "Schmoe")
  
  metadata = rets.GetMetadata()
  system = metadata.GetSystem()
  print "System ID: " + system.GetSystemID()
  print "Description: " + system.GetSystemDescription()
  print "Comments: " + system.GetComments()
  
#  resources = metadata.GetAllResources()
#  for resource in resources:
#    print resource.GetStandardName()
 
  logout = rets.Logout();
except librets.RetsException,e:
  print "Caught: " + e.GetMessage()

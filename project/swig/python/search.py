#!/usr/bin/env python

import librets

try:
  rets = librets.RetsSession("http://demo.crt.realtors.org:6103/rets/login")
  rets.Login("Joe", "Schmoe")
  print "Action: ", rets.GetAction()
  
  request = rets.CreateSearchRequest("Property", "ResidentialProperty",
    "(ListPrice=300000-)")
    
  request.SetSelect("ListingID,ListPrice,Beds,City")
  results = rets.Search(request)
  
  columns = results.GetColumns()
  while results.HasNext():
    for column in columns:
      print column + ": " + results.GetString(column)
    print
  
  logout = rets.Logout();
  print "Billing info: " + logout.GetBillingInfo()
  print "Logout message: " + logout.GetLogoutMessage()
  print "Connect time: " + str(logout.GetConnectTime())
except librets.RetsException,e:
  print "Caught: " + e.GetMessage()

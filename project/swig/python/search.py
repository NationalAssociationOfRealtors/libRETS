#!/usr/bin/env python

import librets

try:
  rets = librets.RetsSession("http://demo.crt.realtors.org:6103/rets/login")
  rets.Login("Joe", "Schmoe")
  print "Action: ", rets.GetAction()
  
  request = rets.CreateSearchRequest("Property", "ResidentialProperty",
    "(ListPrice=300000-)")
    
  request.SetSelect("ListingID,ListPrice,Beds,City")
  request.SetLimit(librets.SearchRequest.LIMIT_DEFAULT)
  request.SetOffset(librets.SearchRequest.OFFSET_NONE)
  request.SetCountType(librets.SearchRequest.RECORD_COUNT_AND_RESULTS)
  results = rets.Search(request)
  
  print "Record count: " + `results.GetCount()`
  print
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

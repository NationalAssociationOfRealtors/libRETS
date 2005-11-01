#!/usr/bin/env python

import librets

try:
  rets = librets.RetsSession("http://demo.crt.realtors.org:6103/rets/login")
  rets.Login("Joe", "Schmoe")
  print "Action: ", rets.GetAction()
  
  request = rets.CreateSearchRequest("Property", "ResidentialProperty",
    "(ListPrice=300000-)")
    
  results = rets.Search(request)
  while results.HasNext():
    print "ListingID: " + results.GetString("ListingID")
    print "ListPrice: " + results.GetString("ListPrice")
    print "Beds: " + results.GetString("Beds")
    print "City: " + results.GetString("City")
    print
  
  logout = rets.Logout();
  print "Billing info: " + logout.GetBillingInfo()
  print "Logout message: " + logout.GetLogoutMessage()
  print "Connect time: " + str(logout.GetConnectTime())
except librets.RetsException,e:
  print "Caught: " + e.GetMessage()

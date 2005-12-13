#!/usr/bin/env ruby

require 'librets'
require 'pp'

include Librets

rets = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

if !rets.Login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

puts "Action: " + rets.GetAction()
version = "1.0"
version = "1.5" if (rets.GetDetectedRetsVersion() == RETS_1_5)
puts "RETS Version: " + version

request = rets.CreateSearchRequest("Property", "ResidentialProperty",
                                   "(ListPrice=300000-)")
request.SetSelect("ListingID,ListPrice,Beds,City")                                     
request.SetLimit(SearchRequest::LIMIT_DEFAULT)
request.SetOffset(SearchRequest::OFFSET_NONE)
request.SetCountType(SearchRequest::RECORD_COUNT_AND_RESULTS);
results = rets.Search(request)

puts "Record count: " + results.GetCount.to_s
puts
columns = results.GetColumns
while results.HasNext()
  columns.each do |column|
    puts column + ": " + results.GetString(column)
  end
  puts
end

logout = rets.Logout()

puts "Billing info: " + logout.GetBillingInfo
puts "Logout message: " + logout.GetLogoutMessage
puts "Connect time: " + logout.GetConnectTime.to_s
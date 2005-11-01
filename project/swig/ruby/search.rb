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

request = rets.CreateSearchRequest("Property", "ResidentialProperty",
                                   "(ListPrice=300000-)")
                                     
results = rets.Search(request)

#columns = results.GetColumns
columns = %w{ListingID ListPrice Beds City}
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
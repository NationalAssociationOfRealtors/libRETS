#!/usr/bin/env ruby

require 'librets'

include Librets

session = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

if !session.login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

puts "Action: " + session.action
version = "1.0"
version = "1.5" if (session.detected_rets_version == RETS_1_5)
puts "RETS Version: " + version

request = session.create_search_request("Property", "ResidentialProperty",
                                        "(ListPrice=300000-)")
request.standard_names = true
request.select = "ListingID,ListPrice,Beds,City"
request.limit = SearchRequest::LIMIT_DEFAULT
request.offset = SearchRequest::OFFSET_NONE
request.count_type = SearchRequest::RECORD_COUNT_AND_RESULTS
request.standard_names = true;
results = session.search(request)

puts "Record count: " + results.count.to_s
puts
columns = results.columns
results.each do |result|
  columns.each do |column|
    puts column + ": " + result.string(column)
  end
  puts
end

logout = session.logout

puts "Billing info: " + logout.billing_info
puts "Logout message: " + logout.logout_message
puts "Connect time: " + logout.connect_time.to_s

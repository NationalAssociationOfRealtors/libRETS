#!/usr/bin/env ruby

require 'librets'
require 'pp'

include Librets

session = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

if !session.login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

puts "Member name: " + session.login_response.member_name
puts "Search URL: " + session.capability_urls.search_url

puts "Action: " + session.action
version = "1.0"
version = "1.5" if (session.detected_rets_version == RETS_1_5)
puts "RETS Version: " + version

logout = session.logout()

puts "Billing info: " + logout.billing_info
puts "Logout message: " + logout.logout_message
puts "Connect time: " + logout.connect_time.to_s
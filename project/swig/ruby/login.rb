#!/usr/bin/env ruby

require 'librets'
require 'pp'

include Librets

session = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

if !session.Login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

puts "Member name: " + session.GetLoginResponse().GetMemberName()
puts "Search URL: " + session.GetCapabilityUrls().GetSearchUrl()

puts "Action: " + session.GetAction()
version = "1.0"
version = "1.5" if (session.GetDetectedRetsVersion() == RETS_1_5)
puts "RETS Version: " + version

logout = session.Logout()

puts "Billing info: " + logout.GetBillingInfo
puts "Logout message: " + logout.GetLogoutMessage
puts "Connect time: " + logout.GetConnectTime.to_s
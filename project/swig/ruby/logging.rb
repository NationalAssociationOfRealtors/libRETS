#!/usr/bin/env ruby

require 'librets'

# Here is where we start doing our RETS stuff
session =
  Librets::RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

log_file = ARGV[0]
session.SetHttpLogName(log_file)

if !session.login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

logout = session.logout

# If we are using logging at ALL, we must call cleanup on session.
# This will free any native resources that point to a ruby based
# logging object.
session.cleanup

puts "See the logging in file #{log_file}" unless log_file.nil?

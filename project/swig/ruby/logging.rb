#!/usr/bin/env ruby

require 'librets'

# To be able to provide a logger to libRETS, you must have an
# initialize method that calls super and implement a method named
# logHttpData that takes two arguements.
class TestLogger < Librets::RetsHttpLogger
  def initialize(filename = nil)
    super()
    if (filename.nil?)
      @file = $stdout
    else
      @file = File.open(filename, 'a')
    end
    @last_type = INFORMATIONAL
  end

  # This example logHttpData mirrors the functionality of the
  # StreamHttpLogger C++ class that ships with the C++ parts of
  # libRETS.
  def logHttpData(type, data)
    if type == RECEIVED && @last_type != RECEIVED
      @file << "\n<<< Received\n"
    elsif type == SENT && @last_type != SENT
      @file << "\n>>> Sent\n"
    elsif type == INFORMATIONAL
      @file << "* "
    end
    @file << data
    @last_type = type
  end
end

# Here is where we start doing our RETS stuff
session =
  Librets::RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

# Create your logger and pass it to the session.  At this point, you
# shouldn't need to touch the Logger again.
log_file = ARGV[0]
session.SetHttpLogger(TestLogger.new(log_file))

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

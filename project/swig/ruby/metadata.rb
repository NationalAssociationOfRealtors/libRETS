#!/usr/bin/env ruby

require 'librets'
require 'pp'

include Librets

def dump_system(metadata)
  system = metadata.GetSystem
  puts "System ID: " + system.GetSystemID
  puts "Description: " + system.GetSystemDescription
  puts "Comment: " + system.GetComments
end

def dump_all_resources(metadata)
  resources = metadata.GetAllResources
  resources.each { |r| puts r.GetStandardName }
end

rets = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

if !rets.Login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

metadata = rets.GetMetadata
dump_system(metadata)
# dump_all_resources(metadata)

rets.Logout()

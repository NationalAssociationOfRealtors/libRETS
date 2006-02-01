#!/usr/bin/env ruby

require 'librets'
require 'pp'

include Librets

session = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

if !session.Login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

get_object_request = GetObjectRequest::new("Property", "Photo")
get_object_request.AddAllObjects("LN000001")

get_object_response = session.GetObject(get_object_request)

content_type_suffixes = { "image/jpeg" => "jpg"}
while object_descriptor = get_object_response.NextObject
  object_key =  object_descriptor.GetObjectKey
  object_id = object_descriptor.GetObjectId
  content_type = object_descriptor.GetContentType
  description = object_descriptor.GetDescription
  print "#{object_key} object \##{object_id}"
  print ", description: #{description}" if !description.empty?
  puts
  
  suffix = content_type_suffixes[content_type]
  output_file_name = object_key + "-" + object_id.to_s + "." + suffix
  File.open(output_file_name, "wb") do |f|
    f << object_descriptor.GetDataAsString
  end
end

session.Logout()

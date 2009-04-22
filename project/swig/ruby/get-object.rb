#!/usr/bin/env ruby

require 'librets'
require 'pp'

include Librets

begin
    session = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")

    if !session.login("Joe", "Schmoe")
      puts "Invalid login"
      exit 1
    end

    get_object_request = GetObjectRequest.new("Property", "Photo")
    get_object_request.add_all_objects("LN000001")

    get_object_response = session.get_object(get_object_request)

    content_type_suffixes = { "image/jpeg" => "jpg", "image/gif" => "gif", "text/xml" => "xml"}
    get_object_response.each_object do |object_descriptor|
      object_key =  object_descriptor.object_key
      object_id = object_descriptor.object_id
      content_type = object_descriptor.content_type
      description = object_descriptor.description
      print "#{object_key} object \##{object_id}"
      print ", description: #{description}" if !description.empty?
      puts
      
      suffix = content_type_suffixes[content_type]
      output_file_name = object_key + "-" + object_id.to_s + "." + suffix
      File.open(output_file_name, "wb") do |f|
        f << object_descriptor.data_as_string
      end
    end

    session.logout
rescue RetsException => e
    puts e
end


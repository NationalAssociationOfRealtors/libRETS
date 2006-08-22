#!/usr/bin/env ruby

require 'librets'
include Librets

def dump_system(metadata)
  system = metadata.system
  puts "System ID: " + system.system_id
  puts "Description: " + system.system_description
  puts "Comment: " + system.comments
end

def dump_all_resources(metadata)
  puts
  metadata.GetAllResources.each do |resource|
    dump_all_classes(metadata, resource)
  end
  metadata.GetAllResources.each do |resource|
    dump_all_lookups(metadata, resource)
  end
end

def dump_all_classes(metadata, resource)
  resource_name = resource.resource_id;
  metadata.GetAllClasses(resource_name).each do |aClass|
    puts "Resource name: " +  resource_name + " [" + 
      resource.standard_name + "]"
    puts "Class name: " + aClass.class_name + " [" +
      aClass.GetStandardName + "]"
    dump_all_tables(metadata, aClass)
    puts
  end
end

def dump_all_tables(metadata, aClass)
  metadata.all_tables(aClass).each do |table|
    puts "Table name: " + table.system_name + " [" + table.standard_name +
      "]"
  end
end

def dump_all_lookups(metadata, resource)
  resource_name = resource.resource_id();
  metadata.all_lookups(resource_name).each do |lookup|
    puts "Resource name: " +  resource_name + " [" + 
      resource.standard_name + "]"
    puts "Lookup name: " + lookup.lookup_name + " (" +
      lookup.visible_name + ")"
    dump_all_lookup_types(metadata, lookup)
    puts
  end
end

def dump_all_lookup_types(metadata, lookup)
  metadata.all_lookup_types(lookup).each do |lookup_type|
    puts "Lookup value: " + lookup_type.value + " (" +
      lookup_type.short_value + ", " +
      lookup_type.long_value + ")"
  end
end

session = RetsSession.new("http://demo.crt.realtors.org:6103/rets/login")
if (ARGV.length == 1) && (ARGV[0] == "full")
  session.incremental_metadata = false
end

if !session.login("Joe", "Schmoe")
  puts "Invalid login"
  exit 1
end

metadata = session.metadata
dump_system(metadata)
dump_all_resources(metadata)

session.logout()

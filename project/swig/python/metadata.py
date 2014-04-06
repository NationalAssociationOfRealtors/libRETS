#!/usr/bin/env python

import sys
import librets

def dump_system(metadata):
    system = metadata.GetSystem()
    print "System ID: " + system.GetSystemID()
    print "Description: " + system.GetSystemDescription()
    print "Comments: " + system.GetComments()
    
def dump_all_resources(metadata):
    print
    for resource in metadata.GetAllResources():
        dump_all_classes(metadata, resource)
    for resource in metadata.GetAllResources():
        dump_all_lookups(metadata, resource)
        
def dump_all_classes(metadata, resource):
    resource_name = resource.GetResourceID()
    for aClass in metadata.GetAllClasses(resource_name):
        print "Resource name: " + resource_name + " [" + \
            resource.GetStandardName() + "]"
        print "Class name: " + aClass.GetClassName() + " ["  + \
            aClass.GetStandardName() + "]"
        dump_all_tables(metadata, aClass)
        print

def dump_all_tables(metadata, aClass):
    for table in metadata.GetAllTables(aClass):
        print "Table name: " + table.GetSystemName()  + " [" + \
            table.GetStandardName() + "]"

def dump_all_lookups(metadata, resource):
    resource_name = resource.GetResourceID()
    for lookup in metadata.GetAllLookups(resource_name):
        print "Resource name: " + resource_name + " [" + \
            resource.GetStandardName() + "]"
        print "Lookup name: " + lookup.GetLookupName() + " ("  + \
            lookup.GetVisibleName() + ")"
        dump_all_lookup_types(metadata, lookup)
        print

def dump_all_lookup_types(metadata, lookup):
    for lookup_type in metadata.GetAllLookupTypes(lookup):
        print "Lookup value: " + lookup_type.GetValue() + " (" + \
            lookup_type.GetShortValue() + ", " + \
            lookup_type.GetLongValue() + ")"
        
    
try:
  session = librets.RetsSession("http://www.dis.com:6103/rets/login")
  if ((len(sys.argv) == 2) and (sys.argv[1] == "full")):
      session.SetIncrementalMetadata(False)
  if (not session.Login("Joe", "Schmoe")):
      print "Invalid login"
      sys.exit(1)
  
  metadata = session.GetMetadata()
  dump_system(metadata)
  dump_all_resources(metadata)
 
  logout = session.Logout();
except librets.RetsException,e:
  print "Caught: " + e.GetMessage()

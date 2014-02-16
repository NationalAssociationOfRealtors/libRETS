#!/usr/bin/env python3
from argparse import ArgumentParser
import sys

import librets

def dump_system(metadata):
    system = metadata.GetSystem()
    print("System ID: " + system.GetSystemID())
    print("Description: " + system.GetSystemDescription())
    print("Comments: " + system.GetComments())

def dump_all_resources(metadata):
    print()
    for resource in metadata.GetAllResources():
        dump_all_classes(metadata, resource)
    for resource in metadata.GetAllResources():
        dump_all_lookups(metadata, resource)

def dump_all_classes(metadata, resource):
    resource_name = resource.GetResourceID()
    for aClass in metadata.GetAllClasses(resource_name):
        print('Resource name: {} [{}]'.format(
            resource_name, resource.GetStandardName()))
        print('Class name: {} [{}]'.format(
            aClass.GetClassName(), aClass.GetStandardName()))
        dump_all_tables(metadata, aClass)
        print()

def dump_all_tables(metadata, aClass):
    for table in metadata.GetAllTables(aClass):
        print('Table name: {} [{}]'.format(
            table.GetSystemName(), table.GetStandardName()))

def dump_all_lookups(metadata, resource):
    resource_name = resource.GetResourceID()
    for lookup in metadata.GetAllLookups(resource_name):
        print('Resource name: {} [{}]'.format(
            resource_name, resource.GetStandardName()))
        print('Lookup name: {} [{}]'.format(
            lookup.GetLookupName(), lookup.GetVisibleName()))
        dump_all_lookup_types(metadata, lookup)
        print()

def dump_all_lookup_types(metadata, lookup):
    for lookup_type in metadata.GetAllLookupTypes(lookup):
        s = 'Lookup value: {} ({}, {})'
        print(s.format(lookup_type.GetValue(), lookup_type.GetShortValue(),
            lookup_type.GetLongValue()))

try:
    session = librets.RetsSession("http://www.dis.com:6103/rets/login")
    p = ArgumentParser()
    p.add_argument('--full', action='store_true')
    args = p.parse_args()

    if args.full:
        session.SetIncrementalMetadata(False)
    if not session.Login("Joe", "Schmoe"):
        print("Invalid login")
        sys.exit(1)

    metadata = session.GetMetadata()
    dump_system(metadata)
    dump_all_resources(metadata)

    logout = session.Logout()
except librets.RetsException as e:
    print("Caught: " + e.GetMessage())

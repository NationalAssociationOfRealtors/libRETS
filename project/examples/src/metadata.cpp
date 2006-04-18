/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */

#include "librets.h"
#include "Options.h"
#include <iostream>

using namespace librets;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::exception;

void dumpSystem(RetsMetadata * metadata);
void dumpAllResources(RetsMetadata * metadata);
void dumpAllClasses(RetsMetadata * metadata, MetadataResource * resource);
void dumpAllTables(RetsMetadata * metadata, MetadataClass * aClass);
void dumpAllLookups(RetsMetadata * metadata, MetadataResource * resource);

int main(int argc, char * argv[])
{
    try
    {
        Options options;
        if (!options.ParseCommandLine(argc, argv))
        {
            return 0;
        }

        RetsSessionPtr session = options.RetsLogin();
        if (!session)
        {
            cout << "Login failed\n";
            return -1;
        }

        RetsMetadata * metadata = session->GetMetadata();
        dumpSystem(metadata);
        dumpAllResources(metadata);

        session->Logout();
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cout);
        return 1;
    }
    catch (exception & e)
    {
        cout << e.what() << endl;
        return 2;
    }
    return 0;
}

void dumpSystem(RetsMetadata * metadata)
{
    MetadataSystem * system = metadata->GetSystem();
    cout << "System ID: " << system->GetSystemID() << endl;
    cout << "System Description: " << system->GetSystemDescription() << endl;
    cout << "Comments: " << system->GetComments() << endl;
}

void dumpAllResources(RetsMetadata * metadata)
{
    MetadataResourceList resources = metadata->GetAllResources();
    MetadataResourceList::iterator i;
    cout << endl;
    for (i = resources.begin(); i != resources.end(); i++)
    {
        MetadataResource * resource = *i;
        dumpAllClasses(metadata, resource);
    }

    for (i = resources.begin(); i != resources.end(); i++)
    {
        MetadataResource * resource = *i;
        dumpAllLookups(metadata, resource);
    }
}

void dumpAllClasses(RetsMetadata * metadata, MetadataResource * resource)
{
    string resourceName = resource->GetResourceID();
    
    MetadataClassList classes =
        metadata->GetAllClasses(resourceName);
    MetadataClassList::iterator i;
    for (i = classes.begin(); i != classes.end(); i++)
    {
        MetadataClass * aClass = *i;
        cout << "Resource name: " << resourceName << " ["
             << resource->GetStandardName() << "]" << endl;
        cout << "Class name: " << aClass->GetClassName() << " ["
             << aClass->GetStandardName() << "]" << endl;
        dumpAllTables(metadata, aClass);
        cout << endl;
    }
}

void dumpAllTables(RetsMetadata * metadata, MetadataClass * aClass)
{
    MetadataTableList tables = metadata->GetAllTables(aClass);
    MetadataTableList::iterator i;
    for (i = tables.begin(); i != tables.end(); i++)
    {
        MetadataTable * table = *i;
        cout << "Table name: " << table->GetSystemName() << " ["
             << table->GetStandardName() << "]" << endl;
    }
}

void dumpAllLookups(RetsMetadata * metadata, MetadataResource * resource)
{
    string resourceName = resource->GetResourceID();
    
    MetadataLookupList classes =
        metadata->GetAllLookups(resourceName);
    MetadataLookupList::iterator i;
    for (i = classes.begin(); i != classes.end(); i++)
    {
        MetadataLookup * aClass = *i;
        cout << "Resource name: " << resourceName << " ["
             << resource->GetStandardName() << "]" << endl;
        cout << "Lookup name: " << aClass->GetLookupName() << " ("
             << aClass->GetVisibleName() << ")" << endl;
        cout << endl;
    }
}

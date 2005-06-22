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
#include <iostream>

using namespace librets;
using std::string;
using std::vector;
using std::cout;
using std::endl;
using std::auto_ptr;

void dumpSystem(RetsMetadataPtr metadata);
void dumpAllClasses(RetsMetadataPtr metadata);
void dumpAllTables(RetsMetadataPtr metadata, MetadataClassPtr aClass);

int main(int argc, char * argv[])
{
    try
    {
        RetsSessionPtr session(
            new RetsSession("http://demo.crt.realtors.org:6103/rets/login"));
        session->Login("Joe", "Schmoe");

        RetsMetadataPtr metadata = session->GetMetadata();
        dumpSystem(metadata);
        dumpAllClasses(metadata);

        session->Logout();
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cout);
    }
}

void dumpSystem(RetsMetadataPtr metadata)
{
    MetadataSystemPtr system = metadata->GetSystem();
    cout << "System ID: " << system->GetSystemID() << endl;
    cout << "System Description: " << system->GetSystemDescription() << endl;
    cout << "Comments: " << system->GetComments() << endl;
}

void dumpAllClasses(RetsMetadataPtr metadata)
{
    MetadataClassListPtr classes = metadata->GetAllClasses();
    MetadataClassList::iterator i;
    for (i = classes->begin(); i != classes->end(); i++)
    {
        MetadataClassPtr aClass(*i);
        cout << endl << "Class name: " << aClass->GetClassName() << " ["
             << aClass->GetStandardName() << "]" << endl;
        dumpAllTables(metadata, aClass);
    }
}

void dumpAllTables(RetsMetadataPtr metadata, MetadataClassPtr aClass)
{
    MetadataTableListPtr tables = metadata->GetTablesForClass(aClass);
    MetadataTableList::iterator i;
    for (i = tables->begin(); i != tables->end(); i++)
    {
        MetadataTablePtr table(*i);
        cout << "Table name: " << table->GetSystemName() << " ["
             << table->GetStandardName() << "]" << endl;
    }
}

/*
 * Copyright (C) 2009 National Association of REALTORS(R)
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

void dumpSystem(RetsMetadata * metadata, RetsSessionPtr session);
void dumpAllResources(RetsMetadata * metadata);
void dumpAllClasses(RetsMetadata * metadata, MetadataResource * resource);
void dumpAllTables(RetsMetadata * metadata, MetadataResource * resource, MetadataClass * aClass);
void dumpAllLookups(RetsMetadata * metadata, MetadataResource * resource);
void dumpAllLookupTypes(RetsMetadata * metadata, MetadataResource * resource, MetadataLookup * lookup);

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
        dumpSystem(metadata, session);

        cout << " create table lookups (" << endl
         << "     resource_name                 varchar," << endl
         << "     lookup_name                   varchar," << endl
         << "     key_value                     varchar," << endl
         << "     long_description              varchar," << endl
         << "     short_description             varchar" << endl
         << "     ,primary key (resource_name, lookup_name, key_value)" << endl
         << " );" << endl << endl;

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

void dumpSystem(RetsMetadata * metadata, RetsSessionPtr session)
{
    MetadataSystem * system = metadata->GetSystem();

    cout << "/*" << endl;
    cout << " * System ID: " << system->GetSystemID() << endl;
    cout << " * System Description: " << system->GetSystemDescription() << endl;
    cout << " * System Comments: " << system->GetComments() << endl;
    cout << " * System TimeZoneOffset: " << system->GetTimeZoneOffset() << endl;
    cout << " */" << endl << endl;

    cout << " create table system (" << endl
     << "     system_id                     varchar," << endl
     << "     system_description            varchar," << endl
     << "     metadata_version              varchar," << endl
     << "     timezone_offset               varchar" << endl
     << " );" << endl << endl;

    cout << "insert into system (system_id,system_description,metadata_version,timezone_offset)" << endl
     << "    values ('" 
     <<  system->GetSystemID() 
     << "','" 
     <<  system->GetSystemDescription() 
     << "','" 
     <<  session->GetLoginResponse()->GetMetadataVersion()
     << "','"
     <<  system->GetTimeZoneOffset()
     << "');" 
     << endl;
    cout << endl;

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
    MetadataSystem * system = metadata->GetSystem();
    string resourceName = resource->GetResourceID();
    
    MetadataClassList classes = metadata->GetAllClasses(resourceName);
    MetadataClassList::iterator i;

    for (i = classes.begin(); i != classes.end(); i++)
    {
        MetadataClass * aClass = *i;

    cout << "create table " << aClass->GetClassName() << "(" << endl;
    dumpAllTables(metadata, resource, aClass);
    cout << ");" << endl;
    cout << endl;
    }
}

void dumpAllTables(RetsMetadata * metadata, MetadataResource * resource, MetadataClass * aClass)
{
    MetadataTableList tables = metadata->GetAllTables(aClass);
    MetadataTableList::iterator i;

    string keyName = resource->GetKeyField();
    int minimum;
    int maximum;

    for (i = tables.begin(); i != tables.end(); i++)
    {
        MetadataTable * table = *i;
        string itemName(table->GetDBName());

        cout << "    " << itemName; 
        assert (itemName.length() <= 30);
        for (int j = 0; j < 30 - itemName.length(); j++)
            cout << " ";
        
        switch (table->GetDataType())
        {
            case MetadataTable::BOOLEAN:    cout << " boolean";    break;
            case MetadataTable::DATE:       cout << " date";    break;
            case MetadataTable::DATE_TIME:  cout << " timestamp";    break;
            case MetadataTable::TIME:       cout << " time";    break;
            case MetadataTable::TINY:
            case MetadataTable::SMALL:      cout << " smallint";    break;
            case MetadataTable::INT:        cout << " integer";    break;
            case MetadataTable::LONG:       cout << " bigint";    break;
            case MetadataTable::DECIMAL:    cout << " numeric";    
                                            cout << " (" << table->GetMaximumLength() << ","
                                                 << table->GetPrecision() << ")"; break;
            case MetadataTable::CHARACTER:
            default:                        cout << " varchar";    break;
        }
        if (table->GetRequired())
            cout << " not null";
        if (table->IsUnique())
            cout << " unique";
        
        if (table->GetSystemName().compare(keyName) == 0)
            cout << " primary key";

#if 0        
        switch (table->GetInterpretation())
        {
            case MetadataTable::LOOKUP:
            case MetadataTable::LOOKUP_MULTI:
            case MetadataTable::LOOKUP_BITSTRING:
            case MetadataTable::LOOKUP_BITMASK:
                cout << " references lookups (key_value)"; break;
            default: break;
        }
#endif
        /*
         * Add any constraints. This must follow after all attributes for
         * the column have been processed.
         */
        switch (table->GetDataType())
        {
            case MetadataTable::TINY:
            case MetadataTable::SMALL:
            case MetadataTable::INT:
            case MetadataTable::LONG:
            case MetadataTable::DECIMAL:
                
                /*
                 * Gotta play these games because some librets may not
                 * handle decimal and floats.
                 */
                minimum = 0;
                maximum = 0;
                
                try
                {
                    minimum = table->GetMinimum();
                }
                catch (exception e) 
                {
                    minimum = -1;
                };
                
                if (minimum != 0)
                {
                    cout << "," << endl;
                    cout << "        check (" 
                    << itemName 
                    << " >= " 
                    << table->GetMinimumAsString() 
                    << ")";
                }
                
                try
                {
                    maximum = table->GetMaximum();
                }
                catch (exception e) 
                {
                    maximum = -1;
                };
                
                if (maximum != 0)
                {
                    cout << "," << endl;
                    cout << "        check (" 
                    << itemName 
                    << " <= " 
                    << table->GetMaximumAsString() 
                    << ")";
                }
                
            default:    break;
        }
        
        if (i != tables.end() - 1)
            cout << ",";
      
        cout << " /* " << table->GetSystemName() << " */ " << endl;
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
        MetadataLookup * lookup = *i;

        dumpAllLookupTypes(metadata, resource, lookup);
        cout << endl;
    }
}

void psqlEscape( string & buf)
{
    string::size_type curPos( 0 );
    string::size_type oldPos( 0 );

    while( ( curPos = buf.find( "\'", oldPos ) ) != string::npos ) 
    {
        buf.replace( curPos, 1, "\'\'" );
        oldPos = curPos + 2;
    }
}

void dumpAllLookupTypes(RetsMetadata * metadata, MetadataResource * resource, MetadataLookup * lookup)
{
    string resourceName = resource->GetResourceID();
    string lookupName = lookup->GetLookupName();

    string system_id    = metadata->GetSystem()->GetSystemID();

    MetadataLookupTypeList lookupTypes = metadata->GetAllLookupTypes(lookup);
    MetadataLookupTypeList::const_iterator i ;

    for (i = lookupTypes.begin(); i != lookupTypes.end(); i++)
    {
        MetadataLookupType * lookupType = *i;
        string longValue = lookupType->GetLongValue();
        string shortValue = lookupType->GetShortValue();
        psqlEscape (longValue);
        psqlEscape (shortValue);

    cout << "insert into lookups values('" 
         << resourceName << "','" 
         << lookup->GetLookupName() << "','"
         << lookupType->GetValue() << "','"
         << longValue << "','"
         << shortValue << "');" << endl;
    }
}


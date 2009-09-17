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
#include "librets/util.h"
#include <iostream>
#include <iomanip>

#include <boost/algorithm/string.hpp>

using namespace librets;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::setw;

namespace ba = boost::algorithm;
namespace po = boost::program_options;

/*
 * This example demonstrates the RETS update transaction.
 */

int main(int argc, char * argv[])
{
    try
    {
        string record;
        string resourceName;
        string className;
        string updateType;
        int validate; 
        int validateOnly = UpdateRequest::VALIDATE_ONLY;
        
        Options options;
        options.descriptions.add_options()
        ("resource,r", po::value<string>(&resourceName)
         ->default_value("Property"), "Update resource")
        ("class,C", po::value<string>(&className)
         ->default_value("RES"), "Update class")
        ("record",po::value<string>(&record)
         ->default_value("ListingID=LN000005,CloseDate=2009-08-20T00:00:00"),
         "Fields to update- format: Field1=Value1,Field2=Value2[,...]")
        ("validate", po::value<int>(&validate)
         ->default_value(validateOnly), "Validation (0,1,2), default: 2")
        ("update-type", po::value<string>(&updateType)
         ->default_value("Change"), "Update Type")
        ;
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
        
        UpdateRequestAPtr updateRequest = session->CreateUpdateRequest(resourceName, className);

        updateRequest->SetDelimiter("|");
        updateRequest->SetValidateFlag(validate);
        updateRequest->SetUpdateType(updateType);
        
        /*
         * Pull apart the record.
         */
        StringVector lines;
        ba::split(lines, record, ba::is_any_of(","));
    
        for (StringVector::const_iterator line = lines.begin(); line != lines.end(); line++)
        {
            string key;
            string value;
            if (!librets::util::splitField(*line, "=", key, value))
            {
                throw RetsException("Invalid key value pair <" + *line + ">");
            }
            ba::trim(key);
            ba::trim(value);
            updateRequest->SetField(key, value);
        }
        
        /*
         * Perform the update.
         */
        UpdateResponseAPtr results = session->Update(updateRequest.get());
        StringVector columns = results->GetColumns();
        
        while (results->HasNext())
        {
            for (StringVector::const_iterator i = columns.begin(); i != columns.end(); i++)
            {
                string column = *i;
                cout << setw(15) << column << ": "
                << setw(0) << results->GetString(column) << endl;
            }
        }
        
        bool seen = false;
        while (results->HasNextError())
        {
            if (!seen)
            {
                cout << "Errors:" << endl;
                seen = true;
            }
            cout << setw(15) << results->GetErrorFieldName() << setw(0)
                 << ", Error: "
                 << results->GetErrorNumber()
                 << " at offset "
                 << results->GetErrorOffset()
                 << ", Message: "
                 << results->GetErrorText()
                 << endl;
        }

        seen = false;
        while (results->HasNextWarning())
        {
            if (!seen)
            {
                cout << "Warnings:" << endl;
                seen = true;
            }
            cout << setw(15) << results->GetWarningFieldName() << setw(0)
            << ", Warning: "
            << results->GetWarningNumber()
            << " at offset "
            << results->GetWarningOffset()
            << ", Message: "
            << results->GetWarningText()
            << ", Response Required: "
            << results->GetWarningResponseRequired()
            << endl;
        }
        
        session->Logout();
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cerr);
    }
    catch (std::exception & e)
    {
        cerr << e.what() << endl;
    }
}

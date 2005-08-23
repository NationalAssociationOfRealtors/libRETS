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
#include <boost/lexical_cast.hpp>

using namespace librets;
using namespace librets::util;
using namespace std;
using boost::lexical_cast;
namespace po = boost::program_options;

int main(int argc, char * argv[])
{
    try
    {
        string resource;
        string type;
        string resourceEntity;
        string objectId;
        string outputPrefix;
        
        Options options;
        options.descriptions.add_options()
            ("resource,r", po::value<string>(&resource)
             ->default_value("Property"), "Object resource")
            ("type,t", po::value<string>(&type)
             ->default_value("Photo"), "Object type")
            ("resource-entity,e", po::value<string>(&resourceEntity)
             ->default_value("LN000001"), "Search select")
            ("object-id,i", po::value<string>(&objectId)
             ->default_value("*"), "Object ID")
            ("output-prefix,P", po::value<string>(&outputPrefix)
             ->default_value(""), "Output file prefix")
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
    
        GetObjectRequestPtr getObjectRequest(
            new GetObjectRequest(resource, type));
    
        if (objectId == "*")
        {
            getObjectRequest->AddAllObjects(resourceEntity);
        }
        else
        {
            getObjectRequest->AddObject(resourceEntity,
                                        lexical_cast<int>(objectId));
        }
    
        GetObjectResponsePtr getObjectResponse =
            session->GetObject(getObjectRequest);
    
        StringMap contentTypeSuffixes;
        contentTypeSuffixes["image/jpeg"] = "jpg";
        ObjectDescriptorPtr objectDescriptor;
        while (objectDescriptor = getObjectResponse->NextObject())
        {
            string objectKey = objectDescriptor->GetObjectKey();
            int objectId = objectDescriptor->GetObjectId();
            string contentType = objectDescriptor->GetContentType();
            string description = objectDescriptor->GetDescription();
            cout << objectKey << " object #" << objectId;
            if (!description.empty())
                cout << ", description: " << description;
            cout << endl;

            string suffix = contentTypeSuffixes[contentType];
            string outputFileName = outputPrefix + objectKey + "-" +
                lexical_cast<string>(objectId) + "." + suffix;
            ofstream outputStream(outputFileName.c_str());
            istreamPtr inputStream = objectDescriptor->GetData();
            readUntilEof(*inputStream, outputStream);
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

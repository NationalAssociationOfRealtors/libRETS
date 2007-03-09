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
#include <boost/algorithm/string.hpp>

using namespace librets;
using namespace librets::util;
using namespace std;
using boost::lexical_cast;
namespace po = boost::program_options;
namespace ba = boost::algorithm;

int main(int argc, char * argv[])
{
    try
    {
        string resource;
        string type;
        string resourceEntity;
        string objectId;
        string outputPrefix;
        vector<string> resourceSets;
        
        Options options;
        options.descriptions.add_options()
            ("resource,r", po::value<string>(&resource)
             ->default_value("Property"), "Object resource")
            ("type,t", po::value<string>(&type)
             ->default_value("Photo"), "Object type")
            ("output-prefix,P", po::value<string>(&outputPrefix)
             ->default_value(""), "Output file prefix")
            ("resource-set,R", po::value< vector<string> >(&resourceSets),
             "Resource sets (e.g. 'resource-id' or 'resource-id:#,#'))")
            ;
        if (!options.ParseCommandLine(argc, argv))
        {
            return 1;
        }
        
        if (resourceSets.size() == 0)
        {
            resourceSets.push_back("LN000001");
        }

        RetsSessionPtr session = options.RetsLogin();
        if (!session)
        {
            cout << "Login failed\n";
            return -1;
        }
    
        GetObjectRequest getObjectRequest(resource, type);
    
        vector<string>::const_iterator i;
        for (i = resourceSets.begin(); i != resourceSets.end(); i++)
        {
            vector<string> resourceSet;
            ba::split(resourceSet, *i, ba::is_any_of(":"));
            if (resourceSet.size() == 1)
            {
                getObjectRequest.AddAllObjects(resourceSet[0]);
            }
            else if (resourceSet.size() == 2)
            {
                vector<string> ids;
                ba::split(ids, resourceSet[1], ba::is_any_of(","));
                vector<string>::const_iterator idString;
                for (idString = ids.begin(); idString != ids.end();
                     idString++)
                {
                    int id  = lexical_cast<int>(*idString);
                    getObjectRequest.AddObject(resourceSet[0], id);
                }
            }
        }
    
        GetObjectResponseAPtr getObjectResponse =
            session->GetObject(&getObjectRequest);
    
        StringMap contentTypeSuffixes;
        contentTypeSuffixes["image/jpeg"] = "jpg";
        ObjectDescriptor * objectDescriptor;
        while ((objectDescriptor = getObjectResponse->NextObject()))
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
            istream * inputStream = objectDescriptor->GetDataStream();
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

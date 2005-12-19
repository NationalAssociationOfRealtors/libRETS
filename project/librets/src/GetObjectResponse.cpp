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
/*
 *  GetObjectResponse.cpp
 *  librets
 *
 *  Created by Dave Dribin on 4/1/05.
 *  Copyright 2005 __MyCompanyName__. All rights reserved.
 *
 */

#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "librets/GetObjectResponse.h"
#include "librets/RetsHttpResponse.h"
#include "librets/ObjectDescriptor.h"
#include "librets/RetsException.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::istream;
using std::ostringstream;
using std::istringstream;
using boost::lexical_cast;
namespace ba = boost::algorithm;

static void getlineCRLF(istream & in, string & line)
{
    line.clear();
    while (!in.eof())
    {
        char ch;
        in.get(ch);
        if (ch == '\r')
        {
            in.get(ch);
            if (ch == '\n')
            {
                return;
            }
            line.append(1, '\r').append(1, ch);
        }
        else
        {
            line.append(1, ch);
        }
    }
}

GetObjectResponse::GetObjectResponse()
{
    mNextObject = mObjects.begin();
}

GetObjectResponse::~GetObjectResponse()
{
}

void GetObjectResponse::Parse(RetsHttpResponsePtr httpResponse)
{
    if (httpResponse->GetResponseCode() != 200)
    {
        ostringstream message;
        message << "Invalid response code: ";
        message << httpResponse->GetResponseCode();
        throw RetsException(message.str());
    }
    string contentType = httpResponse->GetContentType();
    if (ba::starts_with(contentType, "text/xml"))
    {
        // Assume no object found, so do nothing.
        // Todo: check RETS reply code.
    }
    else if (ba::starts_with(contentType, "multipart/parallel"))
    {
        ParseMultiPart(httpResponse);
    }
    else
    {
        ParseSinglePart(httpResponse);
    }
    
    mNextObject = mObjects.begin();
}

void GetObjectResponse::ParseSinglePart(RetsHttpResponsePtr httpResponse)
{
    ObjectDescriptorPtr descriptor(new ObjectDescriptor());
    descriptor->SetContentType(httpResponse->GetContentType());
    descriptor->SetObjectKey(httpResponse->GetHeader("Content-ID"));
    string objectId = httpResponse->GetHeader("Object-ID");
    descriptor->SetObjectId(lexical_cast<int>(objectId));
    descriptor->SetDescription(httpResponse->GetHeader("Content-Description"));
    descriptor->SetData(httpResponse->GetInputStream());
    descriptor->SetLocationUrl(httpResponse->GetHeader("Location"));
    mObjects.push_back(descriptor);
}

void GetObjectResponse::ParseMultiPart(RetsHttpResponsePtr httpResponse)
{
    string contentType = httpResponse->GetContentType();
    string boundary = FindBoundary(contentType);
    string delimiter = "\r\n--";
    delimiter.append(boundary).append("\r\n");

    string closeDelimiter = "\r\n--";
    closeDelimiter.append(boundary).append("--\r\n");

    string content = readIntoString(*httpResponse->GetInputStream());
    
    StringVector parts;
    string::size_type partStart;
    string::size_type partEnd;
    
    partStart = content.find(delimiter);
    if (partStart == string::npos)
    {
        throw RetsException("First delimeter not found");
    }
    partStart += delimiter.size();
    
    bool done = false;
    while (!done)
    {
        partEnd = content.find(delimiter, partStart);
        if (partEnd == string::npos)
        {
            partEnd = content.find(closeDelimiter, partStart);
            if (partEnd == string::npos)
            {
                throw RetsException("Cound not find another delilmiter");
            }
            done = true;
        }

        parts.push_back(string(content, partStart, (partEnd - partStart)));
        partStart = partEnd + delimiter.size();
    }
    
    StringVector::iterator i;
    for (i = parts.begin(); i != parts.end(); i++)
    {
        istreamPtr partStream( new istringstream(*i));
        ParsePartStream(partStream);
    }
}

void GetObjectResponse::ParsePartStream(istreamPtr in)
{
    ObjectDescriptorPtr objectDescriptor(new ObjectDescriptor());
    string line;
    while (true)
    {
        getlineCRLF(*in, line);
        if (line.empty())
        {
            break;
        }
        string name;
        string value;
        if (!splitField(line, ":", name, value))
        {
            throw RetsException("Malformed header: " + line);
        }
        ba::trim(value);
        if (name == "Content-Type")
        {
            objectDescriptor->SetContentType(value);
        }
        else if (name == "Content-ID")
        {
            objectDescriptor->SetObjectKey(value);
        }
        else if (name == "Object-ID")
        {
            objectDescriptor->SetObjectId(lexical_cast<int>(value));
        }
        else if (name == "Location")
        {
            objectDescriptor->SetLocationUrl(value);
        }
        else if (name == "Content-Description")
        {
            objectDescriptor->SetDescription(value);
        }
    }
    objectDescriptor->SetData(in);
    mObjects.push_back(objectDescriptor);
}

string GetObjectResponse::FindBoundary(string contentType)
{
    StringVector parameters;
    ba::split(parameters, contentType, ba::is_any_of(";"));
    StringVector::iterator i;
    for (i = parameters.begin(); i != parameters.end(); i++)
    {
        string parameter = ba::trim_copy(*i);
        string name;
        string boundary;
        if (splitField(parameter, "=", name, boundary) && (name == "boundary"))
        {
            ba::trim(boundary);
            
            // Strip off leading and trailing quote, if it exists
            string::size_type quotePosition = boundary.find_first_of("\"");
            if (quotePosition == string::size_type(0))
            {
                boundary.erase(0, 1);
            }
            
            quotePosition = boundary.find_last_of("\"");
            if (quotePosition == (boundary.size() - 1))
            {
                boundary.erase(boundary.size() - 1, 1);
            }
            return boundary;
        }
    }
    
    LIBRETS_THROW(
        RetsException, ("Could not determine boundary: " + contentType));
}

ObjectDescriptor * GetObjectResponse::NextObject()
{
    ObjectDescriptorPtr nextObject;
    if (mNextObject != mObjects.end())
    {
        nextObject = *mNextObject;
        mNextObject++;
    }
    return nextObject.get();
}


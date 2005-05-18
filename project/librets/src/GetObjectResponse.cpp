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
    if (contentType == "text/xml")
    {
        // Assume no object found, so do nothing.
        // Todo: check RETS reply code.
    }
    else if (startsWith(contentType, "multipart/parallel"))
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
    descriptor->SetObjectId(intValue(objectId));
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
        StringVectorPtr header = split(line, ":", 2);
        if (header->size() != 2)
        {
            throw RetsException("Malformed header: " + line);
        }
        string name = header->at(0);
        string value = trim(header->at(1));
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
            objectDescriptor->SetObjectId(intValue(value));
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
    string boundary;
    StringVectorPtr parameters = split(contentType, ";");
    StringVector::iterator i;
    for (i = parameters->begin(); i != parameters->end(); i++)
    {
        string parameter = trim(*i);
        StringVectorPtr field = split(parameter, "=", 2);
        if ((field->size() == 2) && (field->at(0) == "boundary"))
        {
            boundary = trim(field->at(1));
            
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
        }
    }
    
    return boundary;
}

ObjectDescriptorPtr GetObjectResponse::NextObject()
{
    ObjectDescriptorPtr nextObject;
    if (mNextObject != mObjects.end())
    {
        nextObject = *mNextObject;
        mNextObject++;
    }
    return nextObject;
}


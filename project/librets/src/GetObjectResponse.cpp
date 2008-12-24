/*
 * Copyright (C) 2005-2007 National Association of REALTORS(R)
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

#include <sstream>
#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include "librets/GetObjectResponse.h"
#include "librets/RetsHttpResponse.h"
#include "librets/ObjectDescriptor.h"
#include "librets/RetsException.h"
#include "librets/RetsHttpException.h"
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
    mDefaultsAreValid = false;
    mResponseCode = -1;
    mErrorText = "";
}

GetObjectResponse::~GetObjectResponse()
{
}

void GetObjectResponse::SetDefaultObjectKeyAndId(string defaultObjectKey,
                                                 int defaultObjectId)
{
    mDefaultObjectKey = defaultObjectKey;
    mDefaultObjectId = defaultObjectId;
    mDefaultsAreValid = true;
}

int GetObjectResponse::GetHttpResponse()
{
    return mResponseCode;
}

std::string GetObjectResponse::GetErrorText()
{
    return mErrorText;
}

void GetObjectResponse::SetHttpResponse(int responseCode, std::string errorText)
{
    mResponseCode = responseCode;
    mErrorText = errorText;
}


void GetObjectResponse::Parse(RetsHttpResponsePtr httpResponse,
                              bool ignoreMalformedHeaders)
{
    if (httpResponse->GetResponseCode() != 200)
    {
        ostringstream message;
        message << "Unexpected HHTP response code: ";
        message << httpResponse->GetResponseCode();
        LIBRETS_THROW(RetsHttpException, (httpResponse->GetResponseCode(), message.str()));
    }
    string contentType = httpResponse->GetContentType();
    if (ba::starts_with(contentType, "text/xml"))
    {
        // Assume no object found, so do nothing.
        // Todo: check RETS reply code.
    }
    else if (ba::starts_with(contentType, "multipart/parallel"))
    {
        ParseMultiPart(httpResponse, ignoreMalformedHeaders);
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

    string objectKey = httpResponse->GetHeader("Content-ID");
    if (objectKey.empty() && !mDefaultsAreValid)
    {
        LIBRETS_THROW(RetsException, ("Found empty Content-ID"));
    }
    else if (objectKey.empty() && mDefaultsAreValid)
        descriptor->SetObjectKey(mDefaultObjectKey);
    else
        descriptor->SetObjectKey(objectKey);

    string objectId = httpResponse->GetHeader("Object-ID");
    ba::trim(objectId);

    if (objectId.empty() && !mDefaultsAreValid)
    {
        LIBRETS_THROW(RetsException, ("Found empty Object-ID"));
    }
    else if (objectId.empty() && mDefaultsAreValid)
        descriptor->SetObjectId(mDefaultObjectId);
    else
        descriptor->SetObjectId(lexical_cast<int>(objectId));

    descriptor->SetDescription(httpResponse->GetHeader("Content-Description"));
    descriptor->SetDataStream(httpResponse->GetInputStream());
    descriptor->SetLocationUrl(httpResponse->GetHeader("Location"));
    mObjects.push_back(descriptor);
}

void GetObjectResponse::ParseMultiPart(RetsHttpResponsePtr httpResponse,
                                       bool ignoreMalformedHeaders)
{
    string contentType = httpResponse->GetContentType();
    string boundary = FindBoundary(contentType);
    string delimiter = "\r\n--";
    delimiter.append(boundary).append("\r\n");

    string closeDelimiter = "\r\n--";
    closeDelimiter.append(boundary).append("--");

    string content = readIntoString(httpResponse->GetInputStream());
    
    StringVector parts;
    string::size_type partStart;
    string::size_type partEnd;
    
    partStart = content.find(delimiter);
    if (partStart == string::npos)
    {
        LIBRETS_THROW(RetsException,
                      ("First delimiter not found: " + boundary));
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
                LIBRETS_THROW(
                    RetsException,
                    ("Cound not find another delimiter: " + boundary));
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
        ParsePartStream(partStream, ignoreMalformedHeaders);
    }
}

void GetObjectResponse::ParsePartStream(istreamPtr in,
                                        bool ignoreMalformedHeaders)
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
        if (!splitField(line, ":", name, value) && !ignoreMalformedHeaders)
        {
            LIBRETS_THROW(RetsException, ("Malformed header: " + line));
        }
        ba::trim(value);
        string name_lower(ba::to_lower_copy(name));
        if (name_lower == "content-type")
        {
            objectDescriptor->SetContentType(value);
        }
        else if (name_lower == "content-id")
        {
            objectDescriptor->SetObjectKey(value);
        }
        else if (name_lower == "object-id")
        {
            objectDescriptor->SetObjectId(lexical_cast<int>(value));
        }
        else if (name_lower == "location")
        {
            objectDescriptor->SetLocationUrl(value);
        }
        else if (name_lower == "content-description")
        {
            objectDescriptor->SetDescription(value);
        }
    }
    objectDescriptor->SetDataStream(in);
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


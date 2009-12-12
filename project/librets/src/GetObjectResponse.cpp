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
#include "librets/ExpatXmlParser.h"
#include "librets/RetsXmlStartElementEvent.h"
#include "librets/RetsXmlEndElementEvent.h"
#include "librets/RetsXmlTextEvent.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ios;
using std::istream;
using std::ostringstream;
using std::istringstream;
using boost::lexical_cast;
namespace b = boost;
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

    string contentType(ba::to_lower_copy(httpResponse->GetContentType()));

    if (ba::starts_with(contentType, "text/xml"))
    {
        // Assume no object found, so do nothing.
        // Todo: check RETS reply code.
        ObjectDescriptorPtr objectDescriptor(new ObjectDescriptor());
        istreamPtr in = httpResponse->GetInputStream();
        ios::pos_type current = in->tellg();
        ParseXmlResponse(in, objectDescriptor);
        in->clear();
        in->seekg(current);        
        
        objectDescriptor->SetContentType(httpResponse->GetContentType());
        objectDescriptor->SetDescription(httpResponse->GetHeader("Content-Description"));
        objectDescriptor->SetLocationUrl(httpResponse->GetHeader("Location"));
        objectDescriptor->SetDataStream(in);
        mObjects.push_back(objectDescriptor);
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
    {
        descriptor->SetWildIndicator(objectId == "*");
        try
        {
            descriptor->SetObjectId(lexical_cast<int>(objectId));
        }
        catch (std::exception &e)
        {
            if (mDefaultsAreValid)
                descriptor->SetObjectId(mDefaultObjectId);
            else
                descriptor->SetObjectId(0);
        }
    }
    
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
    
    /*
     * libCURL appears to have a problem when handling a chunked encoded multi-part
     * response where the chunk length follows the CRLF that delineates the message
     * text from the message headers. That CRLF according to the RFCs is supposed to
     * be considered as part of the boundary, but in this case, libCURL fails to
     * include it in the data. So, we need to check to see if our content starts
     * with a boundary missing that leading CRLF.
     */
    if (ignoreMalformedHeaders && !(content.substr(0,2) == "\r\n"))
    {
        
        content = "\r\n" + content;
    }

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
                if (!ignoreMalformedHeaders)
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
            objectDescriptor->SetWildIndicator(value == "*");
            try
            {
                objectDescriptor->SetObjectId(lexical_cast<int>(value));
            }
            catch (std::exception &e)
            {
                if (mDefaultsAreValid)
                    objectDescriptor->SetObjectId(mDefaultObjectId);
                else
                    objectDescriptor->SetObjectId(0);
            }
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

    // If the content-type is text/xml, we need to parse it for RETS ReplyCode.
    string value_lower(ba::to_lower_copy(objectDescriptor->GetContentType()));
    if (value_lower == "text/xml")
    {
        ios::pos_type current = in->tellg();
        ParseXmlResponse(in, objectDescriptor);
        in->clear();
        in->seekg(current);
    }

    objectDescriptor->SetDataStream(in);
    mObjects.push_back(objectDescriptor);
}

void GetObjectResponse::ParseXmlResponse(istreamPtr inputStream, 
                                            ObjectDescriptorPtr objectDescriptor)
{
    ExpatXmlParserPtr XmlParser(new ExpatXmlParser(inputStream, "US-ASCII"));
    
    while (XmlParser->HasNext())
    {
        RetsXmlEventPtr event = XmlParser->GetNextSkippingEmptyText();
        RetsXmlStartElementEventPtr startEvent
            = b::dynamic_pointer_cast<RetsXmlStartElementEvent>(event);
        if (!startEvent)
        {
            continue;
        }
        
        string name = startEvent->GetName();
        if (name == "RETS")
        {
            istringstream replyCodeString(
                startEvent->GetAttributeValue("ReplyCode"));
            int replyCode;
            try
            {
                replyCodeString >> replyCode;
            }
            catch (std::exception &)
            {
                replyCode = 20413; // Miscellaneous Error
            }
            
            if (replyCode != 0)
            {
                string meaning = startEvent->GetAttributeValue("ReplyText");
                objectDescriptor->SetRetsReplyCode(replyCode);
                objectDescriptor->SetRetsReplyText(meaning);
            }
        }
    }
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


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

#include <sstream>
#include <iostream> // $$DEBUG

#include "librets/forward.h"
#include "librets/str_stream.h"
#include "librets/RetsException.h"
#include "librets/UpdateRequest.h"
#include <boost/lexical_cast.hpp>

using namespace librets;

using std::string;
using std::vector;

using boost::lexical_cast;

const char * UpdateRequest::CLASS_PARAMETER = "ClassName";
const char * UpdateRequest::DELIMITER_PARAMETER = "Delimiter";
const char * UpdateRequest::RECORD_PARAMETER = "Record";
const char * UpdateRequest::RESOURCE_PARAMETER = "Resource";
const char * UpdateRequest::UPDATE_TYPE_PARAMETER = "Type";
const char * UpdateRequest::VALIDATE_PARAMETER = "Validate";
const char * UpdateRequest::WARNING_RESPONSE_PARAMETER = "WarningResponse";

UpdateRequest::UpdateRequest(string resourceName, string className)
    : mDelimiter("\t"), mFlag(UpdateRequest::UPDATE_OK), mUpdateType("")
{
    SetQueryParameter(CLASS_PARAMETER, className);
    SetQueryParameter(RESOURCE_PARAMETER, resourceName);
    
    SetDelimiter(mDelimiter);
    SetMethod(POST);
    SetUpdateType(mUpdateType);
    SetValidateFlag(mFlag);
}

const string UpdateRequest::GetDelimiter()
{
    return mDelimiter;
}

string UpdateRequest::ConstructRecord()
{
    string delimiter;
    str_stream record;

    /*
     * Format the "Record" portion of the request.
     */
    delimiter = "";
    for (StringMap::iterator i = mFields.begin(); i != mFields.end(); i++)
    {
        record << delimiter << i->first << "=" << i->second;
        delimiter = mDelimiter;
    }
    return record;
}

void UpdateRequest::ConstructRequest()
{
    SetQueryParameter(RECORD_PARAMETER, ConstructRecord());
    
    SetQueryParameter(WARNING_RESPONSE_PARAMETER, ConstructWarningResponse());
}

string UpdateRequest::ConstructWarningResponse()
{
    string delimiter;
    str_stream warnings;

    /*
     * Format the "WarningResponse portion of the request.
     */
    delimiter = "";
    for (StringMap::iterator i = mWarnings.begin(); i != mWarnings.end(); i++)
    {
        warnings << delimiter << i->first << "=" << i->second;
        delimiter = mDelimiter;
    }
    return warnings;
}

void UpdateRequest::SetDelimiter(string delimiter)
{
    if (delimiter.length() != 1)
        throw RetsException("Invalid delimiter in Update Transaction. Must be 0x00-0xff.");
    
    mDelimiter = delimiter;

    short delimiterChar = (short) delimiter.at(0);
    
    str_stream delimiterHex;
    
    delimiterHex << "0123456789abcdef"[(delimiterChar >> 4) & 0xf];
    delimiterHex << "0123456789abcdef"[delimiterChar & 0xf];
    
    SetQueryParameter(DELIMITER_PARAMETER, delimiterHex);
}

const StringVector UpdateRequest::GetAllFields()
{
    StringVector fields;
    
    for (StringMap::iterator i = mFields.begin(); i != mFields.end(); i++)
    {
        fields.push_back(i->first);
    }
        
    return fields;
}

const string UpdateRequest::GetField(string key)
{   
    return mFields[key];
}

void UpdateRequest::SetField(string key, string value)
{
    mFields[key] = value;
}

const string UpdateRequest::GetUpdateType()
{
    return mUpdateType;
}

void UpdateRequest::SetUpdateType(string updateType)
{
    mUpdateType = updateType;
    SetQueryParameter(UPDATE_TYPE_PARAMETER, mUpdateType);
}

const int UpdateRequest::GetValidateFlag()
{
    return mFlag;
}

void UpdateRequest::SetValidateFlag(int flag)
{
    mFlag = flag;
    SetQueryParameter(VALIDATE_PARAMETER, mFlag);
}

const StringVector UpdateRequest::GetAllWarnings()
{
    StringVector warnings;
    
    for (StringMap::iterator i = mWarnings.begin(); i != mWarnings.end(); i++)
    {
        warnings.push_back(i->first);
    }
        
    return warnings;
}

const string UpdateRequest::GetWarningResponse(int key)
{   
    return GetWarningResponse(lexical_cast<string>(key));
}

const string UpdateRequest::GetWarningResponse(string key)
{   
    return mWarnings[key];
}

void UpdateRequest::SetWarningResponse(int key, string value)
{
    SetWarningResponse(lexical_cast<string>(key), value);
}

void UpdateRequest::SetWarningResponse(string key, string value)
{
    mWarnings[key] = value;
}


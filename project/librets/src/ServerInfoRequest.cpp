/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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
#include "librets/ServerInfoRequest.h"

using namespace librets;
using std::string;

const char * ServerInformationRequest::CLASS_PARAMETER = "Class";
const char * ServerInformationRequest::RESOURCE_PARAMETER = "Resource";
const char * ServerInformationRequest::STANDARD_NAMES_PARAMETER = "StandardNames";

ServerInformationRequest::ServerInformationRequest()
{
    SetClassName("");
    SetResourceName("");
    SetStandardNames(false);
}

void ServerInformationRequest::SetClassName(string  className)
{
    SetQueryParameter(CLASS_PARAMETER, className);
}

void ServerInformationRequest::SetResourceName(string  resourceName)
{
    SetQueryParameter(RESOURCE_PARAMETER, resourceName);
}

void ServerInformationRequest::SetStandardNames(bool standardNames)
{
    SetQueryParameter(STANDARD_NAMES_PARAMETER, standardNames ? "1" : "0");
}

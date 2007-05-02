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

#include <iostream>
#include "librets/DmqlQuery.h"
#include "librets/util.h"
#include "librets/SearchRequest.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostream;

DmqlQuery::DmqlQuery()
    : mLimit(SearchRequest::LIMIT_DEFAULT),
      mOffset(SearchRequest::OFFSET_NONE),
      mCountType(SearchRequest::RECORD_COUNT_AND_RESULTS)
{
    mFields.reset(new StringVector());
}

string DmqlQuery::GetResource() const
{
    return mResource;
}

void DmqlQuery::SetResource(std::string resource)
{
    mResource = resource;
}

string DmqlQuery::GetClass() const
{
    return mClass;
}

void DmqlQuery::SetClass(std::string aClass)
{
    mClass = aClass;
}

void DmqlQuery::AddField(string column)
{
    mFields->push_back(column);
}

StringVector * DmqlQuery::GetFieldsPtr() const
{
    return mFields.get();
}

StringVectorPtr DmqlQuery::GetFields() const
{
    return mFields;
}

DmqlCriterionPtr DmqlQuery::GetCriterion() const
{
    return mCriterion;
}

DmqlCriterion * DmqlQuery::GetCriterionPtr() const
{
    return mCriterion.get();
}

void DmqlQuery::SetCriterion(DmqlCriterionPtr criterion)
{
    mCriterion = criterion;
}

int DmqlQuery::GetLimit() const
{
    return mLimit;
}

void DmqlQuery::SetLimit(int limit)
{
    mLimit = limit;
}

int DmqlQuery::GetOffset() const
{
    return mOffset;
}

void DmqlQuery::SetOffset(int offset)
{
    mOffset = offset;
}

SearchRequest::CountType DmqlQuery::GetCountType() const
{
    return mCountType;
}

void DmqlQuery::SetCountType(SearchRequest::CountType countType)
{
    mCountType = countType;
}

ostream & DmqlQuery::Print(std::ostream & outputStream) const
{
    return outputStream
        << "Resource <" << mResource << ">, Class <" << mClass
        << ">, Fields <" << Output(*mFields) << ">";
}

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
#ifndef LIBRETS_DMQL_QUERY_H
#define LIBRETS_DMQL_QUERY_H
/** 
 * @file DmqlQuery.h
 * (Internal) Contains the DmqlQuery parser class.
 */
 /// @cond MAINTAINER

#include "librets/std_forward.h"
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"
#include "librets/SearchRequest.h"

namespace librets {

class DmqlQuery : public RetsObject
{
  public:
    DmqlQuery();
    virtual ~DmqlQuery();

    std::string GetResource() const;

    void SetResource(std::string resource);

    std::string GetClass() const;

    void SetClass(std::string aClass);

    /**
     * Returns a pointer to a StringVector.  DmqlQuery is responsible for
     * freeing the object.
     */
    StringVector * GetFieldsPtr() const;

    StringVectorPtr GetFields() const;

    void AddField(std::string column);

    /**
     * Returns a pointer to a DmqlCriterion.  DmqlQuery is responsible for
     * freeing the object.
     */
    DmqlCriterion * GetCriterionPtr() const;

    DmqlCriterionPtr GetCriterion() const;

    void SetCriterion(DmqlCriterionPtr criterion);

    int GetLimit() const;

    void SetLimit(int limit);

    int GetOffset() const;

    void SetOffset(int offset);

    SearchRequest::CountType GetCountType() const;

    void SetCountType(SearchRequest::CountType countType);

    virtual std::ostream & Print(std::ostream & outputStream) const;

  private:
    std::string mResource;
    std::string mClass;
    StringVectorPtr mFields;
    DmqlCriterionPtr mCriterion;
    int mLimit;
    int mOffset;
    SearchRequest::CountType mCountType;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

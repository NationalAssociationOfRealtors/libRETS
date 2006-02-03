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

#include "librets/std_forward.h"
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"

namespace librets {

class DmqlQuery : public RetsObject
{
  public:
    DmqlQuery();

    std::string GetResource() const;

    void SetResource(std::string resource);

    std::string GetClass() const;

    void SetClass(std::string aClass);

    StringVectorPtr GetFields() const;

    void AddField(std::string column);

    DmqlCriterionPtr GetCriterion() const;

    void SetCriterion(DmqlCriterionPtr criterion);

    int GetLimit() const;

    void SetLimit(int limit);

    int GetOffset() const;

    void SetOffset(int offset);

    virtual std::ostream & Print(std::ostream & outputStream) const;

  private:
    std::string mResource;
    std::string mClass;
    StringVectorPtr mFields;
    DmqlCriterionPtr mCriterion;
    int mLimit;
    int mOffset;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

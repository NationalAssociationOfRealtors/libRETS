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
#ifndef LIBRETS_FIELD_CRITERION_H
#define LIBRETS_FIELD_CRITERION_H
/** 
 * @file FieldCriterion.h
 * (Internal) Contains the FieldCriterion parser class.
 */
 /// @cond MAINTAINER

#include <string>
#include "librets/sql_forward.h"
#include "librets/DmqlCriterion.h"

namespace librets {

class FieldCriterion : public DmqlCriterion
{
  public:
    FieldCriterion(std::string field, DmqlCriterionPtr value);

    virtual std::ostream & Print(std::ostream & outputStream) const;

    bool Equals(const RetsObject * object) const;

  protected:
    virtual std::string OperationName() const = 0;

    std::string mField;
    DmqlCriterionPtr mValue;
};

};
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

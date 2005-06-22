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
#include "librets/JunctionCriterion.h"
#include "librets/AndCriterion.h"
#include "librets/OrCriterion.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostream;
namespace b = boost;

JunctionCriterion::JunctionCriterion()
    : mCriteria()
{
}

#include <iostream>

void JunctionCriterion::add(DmqlCriterionPtr criterion)
{
    /*
     * Take advantage of the associative law to merge like operations
     * into one operation:
     *
     *  (A + B) + C = A + (B + C) = A + B + C
     *  (A * B) * C = A * (B * C) = A * B * C
     */
    JunctionCriterionPtr junction =
        b::dynamic_pointer_cast<JunctionCriterion>(criterion);
    if (junction)
    {
        /*
         * I could not, for the life of me, get this typeid()
         * comparison to work, even though it seems like it should.
         * typeid() was basically acting non-virtual.
         *
         * if ((typeid(this) == typeid(junction.get())))
         */

        if (OperationName() == junction->OperationName())
        {
            addAll(junction);
        }
        else
        {
            mCriteria.push_back(criterion);
        }
    }
    else
    {
        mCriteria.push_back(criterion);
    }
}


void JunctionCriterion::addAll(JunctionCriterionPtr junction)
{
    addAll(junction->mCriteria);
}

void JunctionCriterion::addAll(const CriterionList & criteria)
{
    CriterionList::const_iterator i;
    for (i = criteria.begin(); i != criteria.end(); i++)
    {
        mCriteria.push_back(*i);
    }
}

ostream & JunctionCriterion::ToDmql(ostream & out) const
{
    out << "(";
    CriterionList::const_iterator i;
    string  separator = "";
    for (i = mCriteria.begin(); i != mCriteria.end(); i++)
    {
        out << separator;
        DmqlCriterionPtr criterion = *i;
        criterion->ToDmql(out);
        separator = Operator();
    }
    return out << ")";
}

ostream & JunctionCriterion::Print(ostream & outputStream) const
{
    return outputStream << "(<" << OperationName() << "> <"
                        << Output(mCriteria) << ">)";
}

#include <iostream>
bool JunctionCriterion::Equals(const RetsObject * object) const
{
    const JunctionCriterion * rhs =
        dynamic_cast<const JunctionCriterion *>(object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (OperationName() == rhs->OperationName());
    equals &= VectorEquals(mCriteria, rhs->mCriteria);
    return equals;
}

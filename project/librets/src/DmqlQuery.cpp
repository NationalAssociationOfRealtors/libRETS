#include <iostream>
#include "librets/DmqlQuery.h"
#include "librets/util.h"

using namespace librets;
using namespace librets::util;
using std::string;
using std::ostream;

DmqlQuery::DmqlQuery()
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

StringVectorPtr DmqlQuery::GetFields() const
{
    return mFields;
}

DmqlCriterionPtr DmqlQuery::GetCriterion() const
{
    return mCriterion;
}

void DmqlQuery::SetCriterion(DmqlCriterionPtr criterion)
{
    mCriterion = criterion;
}

ostream & DmqlQuery::Print(std::ostream & outputStream) const
{
    return outputStream
        << "Resource <" << mResource << ">, Class <" << mClass
        << ">, Fields <" << Output(*mFields) << ">";
}

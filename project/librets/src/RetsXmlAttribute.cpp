#include <iostream>
#include "librets/RetsXmlAttribute.h"

using namespace librets;
using namespace std;

RetsXmlAttribute::RetsXmlAttribute(string name, string value)
{
    mName = name;
    mValue = value;
}

string RetsXmlAttribute::GetName() const
{
    return mName;
}

string RetsXmlAttribute::GetValue() const
{
    return mValue;
}

ostream & RetsXmlAttribute::Print(ostream & outputStream) const
{
    return outputStream << "RetsXmlAttribute[" << mName << "="
                        << mValue << "]";
}

bool RetsXmlAttribute::Equals(const RetsObject * object) const
{
    const RetsXmlAttribute * rhs =
        dynamic_cast<const RetsXmlAttribute *>(object);
    if (rhs == 0)
    {
        return 0;
    }
    return ((mName == rhs->mName) && (mValue == rhs->mValue));
}

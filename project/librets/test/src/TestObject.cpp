#include <iostream>
#include "TestObject.h"

using namespace librets;
using namespace std;

TestObject::~TestObject()
{
}

ostream & TestObject::Print(ostream & outputStream) const
{
    return outputStream << "TestObject: " << mNumber;
}

bool TestObject::Equals(const RetsObject * object) const
{
    const TestObject * rhs = dynamic_cast<const TestObject *>(object);
    if (rhs == 0)
    {
        return false;
    }
    return mNumber == rhs->mNumber;
}

#include <sstream>
#include "librets/RetsXmlEndElementEvent.h"

using namespace librets;
using namespace std;

RetsXmlEndElementEvent::~RetsXmlEndElementEvent()
{
}

RetsXmlEvent::Type RetsXmlEndElementEvent::GetType() const
{
    return END_ELEMENT;
}

void RetsXmlEndElementEvent::SetName(string name)
{
    mName = name;
}

string RetsXmlEndElementEvent::GetName() const
{
    return mName;
}

ostream & RetsXmlEndElementEvent::Print(ostream & outputStream) const
{
    return outputStream << "XML end element: <" << mName << ">";
}

bool RetsXmlEndElementEvent::Equals(const RetsObject * object) const
{
    const RetsXmlEndElementEvent * rhs =
        dynamic_cast<const RetsXmlEndElementEvent *> (object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mName == rhs->GetName());
    return equals;
}

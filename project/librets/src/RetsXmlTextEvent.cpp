#include "librets/RetsXmlTextEvent.h"

using namespace librets;
using namespace std;

RetsXmlTextEvent::~RetsXmlTextEvent()
{
}

RetsXmlEvent::Type RetsXmlTextEvent::GetType() const
{
    return TEXT;
}

void RetsXmlTextEvent::AppendText(string text)
{
    mText << text;
}

string RetsXmlTextEvent::GetText() const
{
    return mText.str();
}

ostream & RetsXmlTextEvent::Print(ostream & outputStream) const
{
    return outputStream << "XML text event, <" << mText.str() << ">";
}

bool RetsXmlTextEvent::Equals(const RetsObject * object) const
{
    const RetsXmlTextEvent * rhs =
        dynamic_cast<const RetsXmlTextEvent *> (object);
    if (rhs == 0)
    {
        return false;
    }

    bool equals = true;
    equals &= (mText.str() == (rhs->mText.str()));
    return equals;
}

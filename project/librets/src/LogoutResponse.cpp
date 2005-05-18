#include <sstream>
#include "librets/LogoutResponse.h"

using namespace librets;
using std::string;
using std::istringstream;

#define CLASS LogoutResponse

void CLASS::ParsingFinished()
{
    istringstream connectTimeString(GetValue("ConnectTime"));
    connectTimeString >> mConnectTime;
}

string CLASS::GetBillingInfo() const
{
    return GetValue("Billing");
}

string CLASS::GetLogoutMessage() const
{
    return GetValue("SignOffMessage");
}

int CLASS::GetConnectTime() const
{
    return mConnectTime;
}

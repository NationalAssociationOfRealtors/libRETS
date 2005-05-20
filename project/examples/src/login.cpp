#include "librets.h"
#include <iostream>

using namespace librets;
using std::string;
using std::cout;
using std::cerr;
using std::endl;

int main(int argc, char * argv[])
{
    try
    {
        string loginUrl = "http://demo.crt.realtors.org:6103/rets/login";
        string username = "Joe";
        string passwd = "Schmoe";

        if (argc > 1)
        {
            loginUrl = argv[1];
        }

        if (argc > 2)
        {
            username = argv[2];
        }

        if (argc > 3)
        {
            passwd = argv[3];
        }

        RetsSessionPtr session(new RetsSession(loginUrl));
        if (!session->Login(username, passwd))
        {
            cout << "Login failed\n";
            return -1;
        }
        cout << "Logged in\n";

        cout << "Action:\n" << session->GetAction() << endl;

        LogoutResponsePtr logout = session->Logout();
        cout << "Logged out\n";
        if (logout)
        {
            cout << "Billing information: " << logout->GetBillingInfo()
                 << endl;
            cout << "Connect time: " << logout->GetConnectTime() << endl;
            cout << "Message: " << logout->GetLogoutMessage() << endl;
        }
    }
    catch (RetsException & e)
    {
        e.PrintFullReport(cerr);
    }
}

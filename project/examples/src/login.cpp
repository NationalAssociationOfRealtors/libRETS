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

#include "librets.h"
#include "Options.h"
#include <iostream>
#include <fstream>

using namespace librets;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::ofstream;

int main(int argc, char * argv[])
{
    try
    {
        Options options;
        if (!options.ParseCommandLine(argc, argv))
        {
            return 0;
        }

        RetsSessionPtr session = options.RetsLogin();
        if (!session)
        {
            cout << "Login failed\n";
            return -1;
        }
        cout << "Logged in\n";

        LoginResponse * login = session->GetLoginResponse();
        cout << "Member name: " << login->GetMemberName() << endl;

        cout << "Action:\n" << session->GetAction() << endl;

        LogoutResponseAPtr logout = session->Logout();
        cout << "Logged out\n";
        if (logout.get())
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
        return 1;
    }
    catch (exception & e)
    {
        cerr << e.what() << endl;
        return 2;
    }
    return 0;
}

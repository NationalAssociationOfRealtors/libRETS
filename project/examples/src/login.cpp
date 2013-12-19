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
#include <iomanip>
#include <fstream>

using namespace librets;
using std::string;
using std::cout;
using std::cerr;
using std::endl;
using std::exception;
using std::ofstream;
using std::setw;

int main(int argc, char * argv[])
{
    try
    {
        Options options;
        options.descriptions.add_options()
            ("show-urls", "Show all the capability URLs")
            ;

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
        CapabilityUrls * urls = session->GetCapabilityUrls();

        cout << "Member name: " << login->GetMemberName() << endl;
        cout << "Search URL: " << urls->GetSearchUrl() << endl;        
        cout << "Action:\n" << session->GetAction() << endl;
        
        if (session->GetDetectedRetsVersion() >= RETS_1_8)
        {
            try
            {
                cout << "User ID:"                          << login->GetUserID() << endl;
                cout << "User Class: "                      << login->GetUserClass() << endl;
                cout << "User Level: "                      << login->GetUserLevel() << endl;
                cout << "Agent Code: "                      << login->GetAgentCode() << endl;
                if (options.count("verbose"))
                {
                    cout << "Broker Code: "                 << login->GetBrokerCode() << endl;
                    cout << "Broker Branch: "               << login->GetBrokerBranch() << endl;
                    cout << "Metadata ID: "                 << login->GetMetadataID() << endl;
                    cout << "Metadata Version: "            << login->GetMetadataVersion() << endl;
                    cout << "Metadata Timestamp: "          << login->GetMetadataTimestamp() << endl;
                    cout << "Min Metadata Timestamp: "      << login->GetMinMetadataTimestamp() << endl;
                    cout << "Balance: "                     << login->GetBalance() << endl;
                    cout << "Timeout Seconds: "             << login->GetTimeout() << endl;
                    cout << "Password Expiration: "         << login->GetPasswordExpire() << endl;
                    cout << "Password Expiration Warn: "    << login->GetWarnPasswordExpirationDays() << endl;
                    cout << "OfficeList: "                  << login->GetOfficeList() << endl;
                    cout << "Standard Names Version: "      << login->GetStandardNamesVersion() << endl;
                    cout << "Vendor Name: "                 << login->GetVendorName() << endl;
                    cout << "Server Product Name: "         << login->GetServerProductName() << endl;
                    cout << "Operator Name: "               << login->GetOperatorName() << endl;
                    cout << "Role Name: "                   << login->GetRoleName() << endl;
                    cout << "Support Contact Information: " << login->GetSupportContactInformation() << endl;
                }
            }
            catch (RetsException & e)
            {
               /*
                * The ServerInformation Transaction is not supported.
                * Continue silently.
                */
            }
        }

        if (options.count("show-urls"))
        {
            /*
             * Action and Search URLs unconditionally shown above.
             */
            cout << "Change Password URL: " << urls->GetChangePasswordUrl() << endl;
            cout << "GetObject URL: " << urls->GetGetObjectUrl() << endl;
            cout << "Login Complete URL: " << urls->GetLoginCompleteUrl() << endl;
            cout << "Logout URL: " << urls->GetLogoutUrl() << endl;
            cout << "GetMetadata URL: " << urls->GetGetMetadataUrl() << endl;
            cout << "ServerInformation URL: " << urls->GetServerInformationUrl() << endl;
            cout << "Update URL: " << urls->GetUpdateUrl() << endl;
            if (session->GetDetectedRetsVersion() >= RETS_1_8)
            {
                cout << "Payload List URL: " << urls->GetPayloadListUrl() << endl;
            }
            cout << endl;
        }
        
        if (session->GetDetectedRetsVersion() == RETS_1_7)
        {
            try
            {
                ServerInformationResponseAPtr serverInfo = session->GetServerInformation();
                
                if (serverInfo.get())
                {
                    StringVector parameters = serverInfo->GetParameters();
                    StringVector::const_iterator i;
                    for (i = parameters.begin(); i != parameters.end(); i++)
                    {
                        if (i->empty())
                        {
                            continue;
                        }
                        cout << *i << ": " << serverInfo->GetValue(*i) << endl;
                    }
                }
            }
            catch (RetsException & e)
            {
               /*
                * The ServerInformation Transaction is not supported.
                * Continue silently.
                */
            }
        }

        if (session->GetDetectedRetsVersion() >= RETS_1_8)
        {
            try
            {
                PayloadListResultSetAPtr payloadList = session->GetPayloadList("");
                    
                if (options.count("verbose"))
                {
                    cout << setw(15) << "Class" << ": "
                    << setw(0) << payloadList->GetPayloadClass() << endl;
                    cout << setw(15) << "Resource" << ": "
                    << setw(0) << payloadList->GetPayloadResource() << endl;
                    cout << setw(15) << "Date" << ": "
                    << setw(0) << payloadList->GetPayloadDate() << endl;
                    cout << setw(15) << "Version" << ": "
                    << setw(0) << payloadList->GetPayloadVersion() << endl << endl;
                }
                
                while (payloadList->HasNext())
                {
                    StringVector columns = payloadList->GetColumns();
                    StringVector::const_iterator i;
                    for (i = columns.begin(); i != columns.end(); i++)
                    {
                        string column = *i;
                        cout << setw(15) << column << ": "
                        << setw(0) << payloadList->GetString(column) << endl;
                    }
                    cout << endl;                    
                }
            }
            catch (RetsException & e)
            {
                /*
                 * The GetPayloadList Transaction is not supported.
                 */
                cout << e.what() << endl;
            }
        }
        
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

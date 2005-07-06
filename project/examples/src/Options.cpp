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

#include "Options.h"
#include "librets.h"
#include <iostream>
#include <fstream>

using namespace librets;
namespace po = boost::program_options;
using std::string;
using std::cout;
using std::endl;
using std::ofstream;

Options::Options()
    : descriptions("Allowed options")
{
    descriptions.add_options()
        ("help,h", "Show help message")
        ("url,u", po::value<string>(&loginUrl)
         ->default_value("http://demo.crt.realtors.org:6103/rets/login", ""),
         "Login URL")
        ("username,U", po::value<string>(&username)->default_value("Joe", ""),
         "User name")
        ("password,p", po::value<string>(&password)
         ->default_value("Schmoe", ""), "Password")
        ("user-agent,a", po::value<string>(&userAgent)
         ->default_value(RetsSession::DEFAULT_USER_AGENT, ""), "User agent")
        ("http-get,g", po::value<bool>(&useHttpGet)
         ->default_value(false, ""), "Use HTTP GET")
        ("http-log,l", po::value<string>(&mLogFile), "HTTP log file")
        ;
}

bool Options::ParseCommandLine(int argc, char * argv[])
{
    po::store(po::parse_command_line(argc, argv, descriptions), options);
    po::notify(options);
    if (options.count("help"))
    {
        cout << descriptions << endl;
        return false;
    }
    return true;
}

RetsSessionPtr Options::RetsLogin()
{
    RetsSessionPtr session(new RetsSession(loginUrl));
    session->SetUserAgent(userAgent);
    session->UseHttpGet(useHttpGet);
    ostreamPtr logFile(new ofstream(mLogFile.c_str()));
    mLogger.reset(new StreamHttpLogger(logFile));
    session->SetHttpLogger(mLogger.get());
    if (!session->Login(username, password))
    {
        session.reset();
    }
    return session;
}

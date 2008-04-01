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
using std::ifstream;

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
        ("ua-password", po::value<string>(&userAgentPassword)
         ->default_value(""), "User agent password")
        ("http-get,g", "Use HTTP GET")
        ("http-log,l", po::value<string>(&mLogFile), "HTTP log file - won't log GetObject calls")
        ("http-log-everything,L", po::value<string>(&mLogFile), "HTTP log file - log GetObject calls")
        ("config-file,c", po::value<string>(&mConfigFile),
         "Use configuration file")
        ("rets-version,V", po::value<string>(&mRetsVersionString)
         ->default_value("1.5", ""), "RETS Version")
        ("full-metadata,F", 
         "Use full metadata (instead of incremental)")
        ;
}

bool Options::ParseCommandLine(int argc, char * argv[])
{
    useHttpGet = false;
    useFullMetadata = false;
    po::store(po::parse_command_line(argc, argv, descriptions), options);
    po::notify(options);

    if (options.count("config-file"))
    {
        ifstream ifs(mConfigFile.c_str());
        po::store(parse_config_file(ifs, descriptions), options);
        po::notify(options);
    }
    if (options.count("http-get"))
    {
        useHttpGet = true;
    }
    if  (options.count("full-metadata"))
    {
        useFullMetadata = true;
    }
    if (options.count("help"))
    {
        cout << descriptions << endl;
        return false;
    }
    if (mRetsVersionString == "1.5")
    {
        retsVersion = RETS_1_5;
    }
    else
    {
        retsVersion = RETS_1_0;
    }
    return true;
}

RetsSessionPtr Options::RetsLogin()
{
    RetsSessionPtr session(new RetsSession(loginUrl));
    session->SetUserAgent(userAgent);
    session->UseHttpGet(useHttpGet);
    session->SetRetsVersion(retsVersion);
    session->SetIncrementalMetadata(!useFullMetadata);
    session->SetUserAgentPassword(userAgentPassword);

    if (options.count("http-log") || options.count("http-log-everything"))
    {
        mLogStream.open(mLogFile.c_str());
        mLogger.reset(new StreamHttpLogger(&mLogStream));
        session->SetHttpLogger(mLogger.get());
	if (options.count("http-log-everything"))
	  session->SetLogEverything(true);
    }
    if (!session->Login(username, password))
    {
        session.reset();
    }
    return session;
}

unsigned Options::count(const char * name) const
{
    return options.count(name);
}

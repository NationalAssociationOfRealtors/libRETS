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
#include "boost/algorithm/string.hpp"

using namespace librets;
namespace po = boost::program_options;
namespace ba = boost::algorithm;
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
        ("user-agent,a", po::value<string>(&userAgent)
         ->default_value(RetsSession::DEFAULT_USER_AGENT, ""), "User agent")
        ("broker-code,b", po::value<string>(&brokerCode)->default_value("", ""),
         "Broker Code")
        ("config-file,c", po::value<string>(&mConfigFile)->default_value("",""),
         "Use configuration file")
        ("disable-streaming,d", "Disable streaming mode")
        ("encoding", po::value<string>(&mEncoding)->default_value("US-ASCII",""),
        "Default character encoding - US-ASCII, ISO or UTF8")
        ("full-metadata,F", 
         "Use full metadata (instead of incremental)")
        ("http-get,g", "Use HTTP GET")
        ("http-log,l", po::value<string>(&mLogFile), "HTTP log file - won't log GetObject calls")
        ("password,p", po::value<string>(&password)
         ->default_value("Schmoe", ""), "Password")
        ("proxy-password,P", po::value<string>(&proxyPassword)->default_value("",""),
         "Proxy Password")
        ("metadata-timestamp,t", po::value<string>(&savedMetadataTimestamp)->default_value("", ""),
         "Saved Metadata Timestamp (RETS 1.7+)")
        ("url,u", po::value<string>(&loginUrl)
         ->default_value("http://demo.crt.realtors.org:6103/rets/login", ""),
         "Login URL")
        ("username,U", po::value<string>(&username)->default_value("Joe", ""),
         "User name")
        ("rets-version,V", po::value<string>(&mRetsVersionString)
         ->default_value("1.5", ""), "RETS Version")
        ("enable-caching", "Enable caching when in streaming mode")
        ("http-log-everything", po::value<string>(&mLogFile), "HTTP log file - log GetObject calls")
        ("proxy-url", po::value<string>(&proxyUrl)->default_value("",""),
         "Proxy URL")
        ("ua-password", po::value<string>(&userAgentPassword)
         ->default_value(""), "User agent password")
        ("no-sslverify", "Do not verify ssl certificate")
        ;
    mLogFile="";
}

bool Options::ParseCommandLine(int argc, char * argv[])
{
    useHttpGet = false;
    useFullMetadata = false;
    disableStreaming = false;
    enableCaching = false;
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
    if (options.count("disable-streaming"))
    {
        disableStreaming = true;
    }
    if (options.count("enable-caching"))
    {
        enableCaching = true;
    }
    if (mRetsVersionString == "1.5")
    {
        retsVersion = RETS_1_5;
    }
    else if (mRetsVersionString == "1.7")
    {
        retsVersion = RETS_1_7;
    }
    else
    {
        retsVersion = RETS_1_0;
    }
    return true;
}

RetsSessionPtr Options::RetsLogin()
{
    unsigned int flags = 0;
    RetsSessionPtr session(new RetsSession(loginUrl));
    session->SetUserAgent(userAgent);
    session->UseHttpGet(useHttpGet);
    session->SetRetsVersion(retsVersion);
    session->SetIncrementalMetadata(!useFullMetadata);
    session->SetUserAgentPassword(userAgentPassword);
    if (disableStreaming)
    {
        flags |= RetsSession::MODE_NO_STREAM;
    }
    if (enableCaching)
    {
        flags |= RetsSession::MODE_CACHE;
    }
    if (options.count("no-sslverify"))
    {
        flags |= RetsSession::MODE_NO_SSL_VERIFY;
    }
    session->SetModeFlags(flags);
    
    if (!proxyUrl.empty())
    {
        session->SetProxy(proxyUrl,proxyPassword);
    }

    string encoding = ba::to_lower_copy(mEncoding);
    if (encoding == "iso")
      session->SetDefaultEncoding(RETS_XML_ISO_ENCODING);
    else
    if (encoding == "utf8")
        session->SetDefaultEncoding(RETS_XML_UTF8_ENCODING);
    
    if (options.count("http-log") || options.count("http-log-everything"))
    {
        mLogStream.open(mLogFile.c_str());
        mLogger.reset(new StreamHttpLogger(&mLogStream));
        session->SetHttpLogger(mLogger.get());
        if (options.count("http-log-everything"))
            session->SetLogEverything(true);
    }
    if (retsVersion == RETS_1_0 || retsVersion == RETS_1_5)
    {
        if (!session->Login(username, password))
        {
            session.reset();
        }
    }
    else
    {
        if (!session->Login(username, password, brokerCode, savedMetadataTimestamp))
        {
            session.reset();
        }
    }

    if (session.get() && session->GetDetectedRetsVersion() != session->GetRetsVersion())
    {
	cout << "** Warning, requested RETS version \"" 
	     << session->RetsVersionToString(session->GetRetsVersion())
	     << "\", got version \""
	     << session->RetsVersionToString(session->GetDetectedRetsVersion())
	     << "\" ** " << endl;
    }

    return session;
}

unsigned Options::count(const char * name) const
{
    return options.count(name);
}

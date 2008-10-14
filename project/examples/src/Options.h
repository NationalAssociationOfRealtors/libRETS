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

#ifndef LIBRETS_EXAMPLES_OPTIONS_H
#define LIBRETS_EXAMPLES_OPTIONS_H

#include <boost/program_options.hpp>
#include "librets.h"
#include <string>
#include <fstream>

class Options
{
  public:
    boost::program_options::options_description descriptions;
    boost::program_options::variables_map options;

    std::string loginUrl;
    std::string username;
    std::string password;
    std::string userAgent;
    std::string userAgentPassword;
    librets::UserAgentAuthType uaAuthType;
    bool useHttpGet;
    librets::RetsVersion retsVersion;
    bool useFullMetadata;
    std::string brokerCode;
    std::string savedMetadataTimestamp;
	std::string proxyUrl;
	std::string proxyPassword;
    bool disableStreaming;
    bool enableCaching;
    std::string mEncoding;
    
    Options();

    bool ParseCommandLine(int argc, char * argv[]);

    librets::RetsSessionPtr RetsLogin();

    unsigned count(const char * name) const;

  private:
    std::string mLogFile;
    std::string mRetsVersionString;
    std::string mUserAgentAuthType;
    std::ofstream mLogStream;
    librets::RetsHttpLoggerPtr mLogger;
    std::string mConfigFile;
};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

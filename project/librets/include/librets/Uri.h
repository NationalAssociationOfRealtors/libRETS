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
#ifndef LIBRETS_URI_H
#define LIBRETS_URI_H

#include <string>
#include "librets/RetsObject.h"

namespace librets {

class Uri : public RetsObject
{
  public:
    static const int DEFAULT_PORT = 80;

    Uri(std::string uri);

    Uri(std::string uri, std::string baseUri);

    bool IsAbsolute() const;

    std::string GetScheme() const;

    void SetScheme(std::string scheme);

    std::string GetHost() const;

    void SetHost(std::string host);

    int GetPort() const;

    void SetPort(int port);

    std::string GetPath() const;

    void SetPath(std::string path);
    
    static std::string Resolve(std::string uri, std::string baseUri);

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * object) const;

  private:
    void init(const std::string & uri);

    bool mIsAbsolute;
    std::string mScheme;
    std::string mHost;
    int mPort;
    std::string mPath;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

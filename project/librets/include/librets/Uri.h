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
/**
 * @file Uri.h
 * (Internal) Contains the Url class definition.
 */
/// @cond MAINTAINER

#include <string>
#include "librets/RetsObject.h"

namespace librets {

class Uri : public RetsObject
{
  public:
    static const int DEFAULT_PORT = 80;
    /**
     * Constructor providing just the uri.
     * @param uri A string containing the uri.
     */
    Uri(std::string uri);
    /**
     * Constructor providing both a base uri and additional
     * uri information. This allows construction of a full uri
     * when only a relative uri is provided.
     * @param uri A string containing the relative uri.
     * @param baseUri A string containg the base uri.
     */
    Uri(std::string uri, std::string baseUri);

    /**
     * Returns whether or not the uri is relative or absolute.
     * @return TRUE if the uri is absolute.
     */
    bool IsAbsolute() const;

    /**
     * Returns the type of the uri, e.g. "http", "ftp", "https".
     * @return A string representing the transport scheme.
     */
    std::string GetScheme() const;

    /**
     * Set the transport scheme.
     * @param scheme A string containing the transport scheme suah as 
     * "http", "ftp", "https", etc.
     */
    void SetScheme(std::string scheme);

    /**
     * Returns the host portion of the uri.
     * @return A string representing the host portion of the uri.
     */
    std::string GetHost() const;
    
    /**
     * Set the host portion of the uri.
     * @param host A string representing the host portion of the uri.
     */
    void SetHost(std::string host);
    
    /**
     * Get the port number for the connection.
     * @return An integer containing the port number. By default this is 80.
     */
    int GetPort() const;

    /**
     * Set the port number for the connection.
     * @param port An integer representing the port number to set.
     */
    void SetPort(int port);

    /**
     * Return the path portion of the uri.
     * @return A string representing the path portion of the uri.
     */
    std::string GetPath() const;

    /**
     * Set the path portion of the uri.
     * @param path A string containing the path.
     */
    void SetPath(std::string path);
    
    /**
     * Given a base uri and path, return a fully formed uri.
     * @param uri An absolute uri or the path portion of a relative uri.
     * @param baseUri The base uri when doing relative paths.
     * @return A string containing a fully formatted uri.
     */
    static std::string Resolve(std::string uri, std::string baseUri);

    /**
     * Prints the object in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;

    /**
     * Checks to see if the uri are equal.
     * @return TRUE if they match.
     */
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
/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

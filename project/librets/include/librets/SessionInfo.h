/*
 * Copyright (C) 2005-2013 National Association of REALTORS(R)
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
#ifndef LIBRETS_SESSION_INFO_H
#define LIBRETS_SESSION_INFO_H
/**
 * @file SessionInfo.h 
 * (Internal) Contains the SessionInfo class definition.
 */
 
 /// @cond MAINTAINER
 
#include "librets/std_forward.h"
#include "librets/xml_forward.h"
#include "librets/EncodingType.h"
#include "librets/RetsObject.h"
#include "librets/RetsVersion.h"

namespace librets {

/**
 * SessionInfo is a class that parses a string in the form of 
 * <code>Info=info-token-name;info-token-type;info-token-value</code>, 
 * saves and enables access to the results.
 */
class SessionInfo : public RetsObject
{
  public:
    /**
     * Default constructor.
     */
    SessionInfo();
    
    virtual ~SessionInfo();

    /**
     * For a given key, return the associated type. If the key is not found
     * an empty string is returned.
     * @param key A string representing the key whose value is to be returned.
     * @return A string containing the value.
     */
    std::string GetType(std::string key) const;

    /**
     * For a given key, return the associated value. If the key is not found
     * an empty string is returned.
     * @param key A string representing the key whose value is to be returned.
     * @return A string containing the value.
     */
    std::string GetValue(std::string key) const;
    
    /**
     * Return all of the tokens in the format "*(tokenname;tokentype;tokenvalueCRLF)"
     * @return A string containing the token values. It is up to the caller to parse
     * them.
     */
    std::string GetSessionInformationTokens() const;

    /**
     * Parse the line as a session information token line.
     */
    void ParseInfo(std::string info);

  private:
    std::string mSessionInformationTokens;
    StringMap mTypes;
    StringMap mValues;
};

};

/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

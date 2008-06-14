/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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

#ifndef LIBRETS_SERVERINFO_REQUEST_H
#define LIBRETS_SERVERINFO_REQUEST_H

#include <string>

#include "librets/RetsHttpRequest.h"

namespace librets {

/**
 * RETS 1.7 Server Information request.
 */
class ServerInformationRequest : public RetsHttpRequest
{
  public:
    /**
     * Create a server information request. This is for RETS 1.7 and later.
     */
    ServerInformationRequest();
    
    /**
     * Set the RETS class name for this request.
     *
     * @param className RETS class name
     */
    void SetClassName(std::string className);

    /**
     * Set the RETS resource name for this request.
     *
     * @param resourceName RETS resource name
     */
    void SetResourceName(std::string resourceName);

    /**
     * Set the flag to indicate whether or not StandardNames are being used.
     *
     * @param standardNames Flag if <code>true</code> indicates that StandardNames is being used.
     */
    void SetStandardNames(bool standardNames);
    

  private:
    static const char * RESOURCE_PARAMETER;
    static const char * CLASS_PARAMETER;
    static const char * STANDARD_NAMES_PARAMETER;

};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

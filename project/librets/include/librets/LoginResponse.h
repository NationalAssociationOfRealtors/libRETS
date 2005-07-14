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

#ifndef LIBRETS_LOGIN_RESPONSE_H
#define LIBRETS_LOGIN_RESPONSE_H

#include "librets/protocol_forward.h"
#include "librets/KeyValueResponse.h"

namespace librets {

class LoginResponse : public KeyValueResponse
{
  public:
    std::string GetMemberName() const;

    std::string GetUserInfo() const;

    std::string GetBroker() const;

    std::string GetMetadataVersion() const;

    std::string GetMetadataTimestamp() const;

    std::string GetMinMetadataTimestamp() const;

    std::string GetOfficeList() const;

    std::string GetBalance() const;

    std::string GetTimeout() const;

    std::string GetPasswordExpire() const;

    std::string GetActionUrl() const;

    std::string GetChangePasswordUrl() const;

    std::string GetGetObjectUrl() const;

    std::string GetLoginUrl() const;

    std::string GetLoginCompleteUrl() const;

    std::string GetLogoutUrl() const;

    std::string GetSearchUrl() const;

    std::string GetGetMetadataUrl() const;

    std::string GetServerInformationUrl() const;

    std::string GetUpdateUrl() const;

    CapabilityUrlsPtr GetCapabilityUrls(std::string baseUrl) const;
    
  private:
    virtual RetsXmlTextEventPtr GetBodyEvent(RetsXmlEventListPtr eventList,
                                             RetsVersion retsVersion);

    CapabilityUrlsPtr mCapabilityUrls;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

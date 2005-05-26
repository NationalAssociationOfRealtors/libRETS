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
    CapabilityUrlsPtr mCapabilityUrls;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

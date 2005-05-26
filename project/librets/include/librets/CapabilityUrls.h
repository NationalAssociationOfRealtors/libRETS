#ifndef LIBRETS_CAPABILITY_URLS_H
#define LIBRETS_CAPABILITY_URLS_H

#include "librets/RetsObject.h"

namespace librets {

class CapabilityUrls : public RetsObject
{
  public:
    CapabilityUrls(std::string baseUrl);

    virtual ~CapabilityUrls();

    void SetActionUrl(std::string actionUrl);

    std::string GetActionUrl() const;

    void SetChangePasswordUrl(std::string changePasswordUrl);

    std::string GetChangePasswordUrl() const;

    void SetGetObjectUrl(std::string getObjectUrl);

    std::string GetGetObjectUrl() const;

    void SetLoginUrl(std::string loginUrl);

    std::string GetLoginUrl() const;

    void SetLoginCompleteUrl(std::string loginCompleteUrl);

    std::string GetLoginCompleteUrl() const;

    void SetLogoutUrl(std::string logoutUrl);

    std::string GetLogoutUrl() const;

    void SetSearchUrl(std::string searchUrl);

    std::string GetSearchUrl() const;

    void SetGetMetadataUrl(std::string getMetadataUrl);

    std::string GetGetMetadataUrl() const;

    void SetServerInformationUrl(std::string serverInformationUrl);

    std::string GetServerInformationUrl() const;

    void SetUpdateUrl(std::string updateUrl);

    std::string GetUpdateUrl() const;

    virtual std::ostream & Print(std::ostream & outputStream) const;

    virtual bool Equals(const RetsObject * object) const;

  private:
    std::string ResolveUrl(std::string url);

    std::string mBaseUrl;
    std::string mActionUrl;
    std::string mChangePasswordUrl;
    std::string mGetObjectUrl;
    std::string mLoginUrl;
    std::string mLoginCompleteUrl;
    std::string mLogoutUrl;
    std::string mSearchUrl;
    std::string mGetMetadataUrl;
    std::string mServerInformationUrl;
    std::string mUpdateUrl;
};


};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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
#ifndef LIBRETS_CAPABILITY_URLS_H
#define LIBRETS_CAPABILITY_URLS_H
/** 
 * @file CapabilityUrls.h
 * Contains the CapabilityUrls class definition.
 */

#include "librets/RetsObject.h"

namespace librets {

/**
 * CapabilityUrls contains the API that allows access to the URLs sent in
 * response to the RETS LOGIN transaction. These URLs are then used by the
 * client code to invoke various RETS operations.
 */
class CapabilityUrls : public RetsObject
{
  public:
    /**
     * Construct and specify the base URL for the RETS server.
     * @param baseUrl A string containing the base URL for the RETS server.
     */
    CapabilityUrls(std::string baseUrl);

    virtual ~CapabilityUrls();

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "ACTION" URL.
     * @param actionUrl A string containing the URL for the ACTION operation.
     */
    void SetActionUrl(std::string actionUrl);
    /// @endcond
    
    /**
     * Get the RETS "ACTION" URL.
     * @return A string containing the URL or an empty string if none exists.
     */
    std::string GetActionUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "CHANGE PASSWORD" URL.
     * @param changePasswordUrl A string containing the URL for the
     * RETS ChangePassword transaction.
     */
    void SetChangePasswordUrl(std::string changePasswordUrl);
    /// @endcond

    /**
     * Get the RETS "CHANGE PASSWORD" URL.
     * @return A string containing the URL for the RETS ChangePassword
     * transaction. This may be the empty string if none exists.
     */
    std::string GetChangePasswordUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "GET OBJECT" URL.
     * @param getObjectUrl A string containing the URL for the
     * RETS GetObject transaction.
     */
    void SetGetObjectUrl(std::string getObjectUrl);
    /// @endcond

    /**
     * Get the RETS "GET OBJECT" URL.
     * @return A string containing the URL for the RETS GetObject
     * tarnsaction. This may be the empty string if none exists.
     */
    std::string GetGetObjectUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "LOGIN" URL.
     * @param loginUrl A string containing the URL for the RETS Login
     * transaction.
     */
    void SetLoginUrl(std::string loginUrl);
    /// @endcond

    /**
     * Get the RETS "LOGIN" URL.
     * @return A string containing the URL for the RETS Login transaction.
     */
    std::string GetLoginUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "LOGIN COMPLETE" URL.
     * @param loginCompleteUrl A string containing the URL for hte RETS
     * Login Complete operation.
     */
    void SetLoginCompleteUrl(std::string loginCompleteUrl);
    /// @endcond

    /**
     * Get the RETS "LOGIN COMPLETE" URL.
     * @return A string containing the URL for the RETS Login Complete operation.
     * Note that this is currently a reserved operation.
     */
    std::string GetLoginCompleteUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "LOGOUT" URL.
     * @param logoutUrl A string containing the URL for the RETS Logout transaction.
     */
    void SetLogoutUrl(std::string logoutUrl);
    /// @endcond

    /**
     * Get the RETS "LOGOUT" URL.
     * @return A string containing the URL for the RETS Logout transaction.
     */
    std::string GetLogoutUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "SEARCH" URL.
     * @param searchUrl A string containing the URL for the RETS Search transaction.
     */
    void SetSearchUrl(std::string searchUrl);
    /// @endcond

    /**
     * Get the RETS "SEARCH" URL.
     * @return A string containing the URL for the RETS Search transaction.
     */
    std::string GetSearchUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "METADATA" URL.
     * @param getMetadataUrl A string containing the URL for the RETS 
     * GetMetadata transaction.
     */
    void SetGetMetadataUrl(std::string getMetadataUrl);
    /// @endcond

    /**
     * Get the RETS "METADATA" URL.
     * @return A string containing the URL for the RETS GetMetadata transaction.
     */
    std::string GetGetMetadataUrl() const;
    
    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "SERVER INFORMATION" URL. This is for RETS 1.7 and later.
     * @param serverInformationUrl A string containing the URL for the RETS
     * ServerInformation transaction.
     */
    void SetServerInformationUrl(std::string serverInformationUrl);
    /// @endcond

    /**
     * Get the RETS "SERVER INFORMATION" URL. This is for RETS 1.7 and later
     * and will return an empty string if it is not implemented.
     * @return A string containing the URL for the RETS ServerInformation
     * transaction. It will be an empty string if not supported.
     */
    std::string GetServerInformationUrl() const;

    /// @cond MAINTAINER
    /**
     * (Internal) Set the RETS "UPDATE" URL.
     * @param updateUrl A string containing the URL for the RETS Update transaction.
     */
    void SetUpdateUrl(std::string updateUrl);
    /// @endcond

    /**
     * Get the RETS "UPDATE" URL.
     * @return A string containing the URL for the RETS Update transaction.
     * It will be an empty string if not supported.
     */
    std::string GetUpdateUrl() const;

    /**
     * Prints the object in a standard form for debugging
     * and error reporting.
     */
    virtual std::ostream & Print(std::ostream & outputStream) const;

    /**
     * Checks to see if the URLs are identical between two CapabilityUrls objects.
     * @return TRUE if the URLs all match.
     */
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

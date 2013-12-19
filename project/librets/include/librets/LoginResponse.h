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
/**
 * @file LoginResponse.h 
 * Contains the LoginResponse class definition.
 */

#include "librets/protocol_forward.h"
#include "librets/KeyValueResponse.h"

namespace librets {
/**
 * LoginResponse contains the API that allows access to the information
 * sent in response to a successful RETS LOGIN transaction.
 */
class LoginResponse : public KeyValueResponse
{
  public:
    /**
     * Get the Member's full name.
     * @return A string containing the Member's name.
     */
    std::string GetMemberName() const;

    /**
     * Get the User specific information.
     * @return A string containing the User specific information.
     */
    std::string GetUserInfo() const;

    /**
     * Get the broker code and branch.
     * @return A string containing the broker code and branch.
     */
    std::string GetBroker() const;

    /**
     * Get the current metadata version.
     * @return A string containing the current metadata version.
     */
    std::string GetMetadataVersion() const;

    /**
     * Get the timestamp of the metadata.
     * @return A string containing the timestamp for the metadata.
     */
    std::string GetMetadataTimestamp() const;

    /**
     * Get the minimum metadata timestamp.
     * @return A string containing the minimum metadata timestamp.
     */
    std::string GetMinMetadataTimestamp() const;

    /**
     * Get the Office List information.
     * @return A string containing the Office List information.
     */
    std::string GetOfficeList() const;

    /**
     * Get the Accounting Information.
     * @return A string containing user readable balance information.
     */
    std::string GetBalance() const;

    /**
     * Get the timeout value.
     * @return A string representing the timeout in seconds.
     */
    std::string GetTimeout() const;

    /**
     * Get the password expiration date.
     * @return A string representing the RFC 1123 format date of the expiration.
     */
    std::string GetPasswordExpire() const;
    
    /**
     * Get the RETS "ACTION" URL.
     * @return A string containing the URL or an empty string if none exists.
     */
    std::string GetActionUrl() const;

    /**
     * Get the RETS "CHANGE PASSWORD" URL.
     * @return A string containing the URL for the RETS ChangePassword
     * transaction. This may be the empty string if none exists.
     */
    std::string GetChangePasswordUrl() const;

    /**
     * Get the RETS "GET OBJECT" URL.
     * @return A string containing the URL for the RETS GetObject
     * tarnsaction. This may be the empty string if none exists.
     */
    std::string GetGetObjectUrl() const;

    /**
     * Get the RETS "LOGIN" URL.
     * @return A string containing the URL for the RETS Login transaction.
     */
    std::string GetLoginUrl() const;

    /**
     * Get the RETS "LOGIN COMPLETE" URL.
     * @return A string containing the URL for the RETS Login Complete operation.
     * Note that this is currently a reserved operation.
     */
    std::string GetLoginCompleteUrl() const;

    /**
     * Get the RETS "LOGOUT" URL.
     * @return A string containing the URL for the RETS Logout transaction.
     */
    std::string GetLogoutUrl() const;

    /**
     * Get the RETS "SEARCH" URL.
     * @return A string containing the URL for the RETS Search transaction.
     */
    std::string GetSearchUrl() const;

    /**
     * Get the RETS "METADATA" URL.
     * @return A string containing the URL for the RETS GetMetadata transaction.
     */
    std::string GetGetMetadataUrl() const;

    /**
     * @deprecated
     * Get the RETS "SERVER INFORMATION" URL. This is for RETS 1.7 and later
     * and will return an empty string if it is not implemented.
     * @return A string containing the URL for the RETS ServerInformation
     * transaction. It will be an empty string if not supported.
     */
    std::string GetServerInformationUrl() const;

    /**
     * Get the RETS "UPDATE" URL.
     * @return A string containing the URL for the RETS Update transaction.
     * It will be an empty string if not supported.
     */
    std::string GetUpdateUrl() const;
    
    /**
     * RETS 1.8
     * Get the "PAYLOADLIST" URL.
     * @return A string containing the URL for the RETS Update transaction.
     * It will be an empty string if not supported.
     */
    std::string GetPayloadListUrl() const;
        
    /**
     * RETS 1.8
     * Get the USERID
     * @return A string containing the USERID
     */
    std::string GetUserID() const;
    
    /**
     * RETS 1.8
     * Get the USERCLASS
     * @return A string containing the USERCLASS
     */
    std::string GetUserClass() const;
    
    /**
     * RETS 1.8
     * Get the USERLEVEL
     * @return A string containing the USERLEVEL
     */
    std::string GetUserLevel() const;
    
    /**
     * RETS 1.8
     * Get the AGENTCODE
     * @return A string containing the AGENTCODE
     */
    std::string GetAgentCode() const;
    
    /**
     * RETS 1.8
     * Get the BROKERCODE
     * @return A string containing the BROKERCODE
     */
    std::string GetBrokerCode() const;
    
    /**
     * RETS 1.8
     * Get the BROKERBRANCH
     * @return A string containing the BROKERBRANCH
     */
    std::string GetBrokerBranch() const;
    
    /**
     * RETS 1.8
     * Get the MetadataID
     * @return A string containing the MetadataID
     */
    std::string GetMetadataID() const;
    
    /**
     * RETS 1.8
     * Get the WarnPasswordExpirationDays
     * @return A string containing the WarnPasswordExpirationDays
     */
    std::string GetWarnPasswordExpirationDays() const;
    
    /**
     * RETS 1.8
     * Get the StandardNamesVersion
     * @return A string containing the StandardNamesVersion
     */
    std::string GetStandardNamesVersion() const;
    
    /**
     * RETS 1.8
     * Get the VendorName
     * @return A string containing the VendorName
     */
    std::string GetVendorName() const;
    
    /**
     * RETS 1.8
     * Get the ServerProductName
     * @return A string containing the ServerProductName
     */
    std::string GetServerProductName() const;
    
    /**
     * RETS 1.8
     * Get the ServerProductVersion
     * @return A string containing the ServerProductVersion
     */
    std::string GetServerProductVersion() const;
    
    /**
     * RETS 1.8
     * Get the OperatorName
     * @return A string containing the OperatorName
     */
    std::string GetOperatorName() const;
    
    /**
     * RETS 1.8
     * Get the RoleName
     * @return A string containing the RoleName
     */
    std::string GetRoleName() const;
    
    /**
     * RETS 1.8
     * Get the SupportContactInformation
     * @return A string containing the SupportContactInformation
     */
    std::string GetSupportContactInformation() const;
    
    /**
     * RETS 1.8
     * Get the Session Information Tokens as a single string.
     * It will be up to the user to parse them.
     * @return A string containing all the seesion information tokens
     * in the format "tokenname;tokentype;tokenvalueCRLF...."
     */
    std::string GetSessionInformationTokens() const;
    
    /// @cond MAINTAINER
    /**
     * (Internal) Create the CapabilityUrl object with the URLs enumerated.
     * @param baseUrl A string containing the base URL for the RETS server.
     */
    CapabilityUrlsAPtr CreateCapabilityUrls(std::string baseUrl) const;
    /// @endcond
    
  private:
    virtual RetsXmlTextEventPtr GetBodyEvent(RetsXmlEventListPtr eventList,
                                             RetsVersion retsVersion);
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

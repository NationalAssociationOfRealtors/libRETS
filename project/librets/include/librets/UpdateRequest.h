/*
 * Copyright (C) 2009 National Association of REALTORS(R)
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

#ifndef LIBRETS_UPDATE_REQUEST_H
#define LIBRETS_UPDATE_REQUEST_H
/**
 * @file UpdateRequest.h
 * Contains the UpdateRequest class definition.
 */
#include <string>

#include "librets/RetsHttpRequest.h"

namespace librets {

/**
 * UpdateRequest contains the API that is used to set up and
 * customer a RETS UPDATE transaciton.
 */
class UpdateRequest : public RetsHttpRequest
{
  public:
    /**
     * Create a new update request.   
     *
     * @param resourceName RETS resource name
     * @param className RETS class name
     */
    UpdateRequest(std::string resourceName, std::string className);
    
    /// @cond MAINTAINER
    /**
     * Format the Record request. This is an internal method, primarily
     * used by the tests.
     */
    std::string ConstructRecord();

    /**
     * Construct the update request. This is an internal method that will
     * format the "Record" and "WarningResponse" elements of the request
     * and set them into the query.
     */
    void ConstructRequest();
    
    /**
     * Format the WarningResponse request. This is an internal method, primarily
     * used by the tests.
     */
    std::string ConstructWarningResponse();
    /// @endcond
    
    /**
     * Returns the current value of the delimiter for the current request.
     *
     * @return A string containing the delimiter.
     */
    const std::string GetDelimiter();
    
    /**
     * Sets the value of the delimiter for the current request.
     *
     * @param delimiter A string containing the delimiter value.
     */
    void SetDelimiter(std::string delimiter);
    
    /**
     * Returns a string vector containing the names of all fields.
     *
     * @return A StringVector containing the names of all fields.
     */
    const StringVector GetAllFields();
    
    /**
     * Returns the value for the given field.
     *
     * @param key A string containing the field name.
     * @return A string containing the field value.
     */
    const std::string GetField(std::string key);
    
    /**
     * Set the name of a field and its value.
     *
     * @param key A string containing the name of the field.
     * @param value A string containing the value for the field.
     */
    void SetField(std::string key, std::string value);
    
    /**
     * Returns the current value of the update parameter for the request.
     *
     * @return A string containing the value of the update type.
     */
    const std::string GetUpdateType();
    
    /**
     * Set the value of the update type parameter for the current request.
     *
     * @param updateType A string containing the value of the update type.
     */
    void SetUpdateType(std::string updateType);

     /**
     * Returns the current value of the update parameter for the request.
     *
     * @return A string containing the value of the action.
     */
    const std::string GetUpdateAction();
    
    /**
     * Set the value of the update action parameter for the current request.
     *
     * @param action A string containing the value of the action.
     */
    void SetUpdateAction(std::string action);
    
    /**
     * This requests the server update the record if there are no errors.
     */
    static const int UPDATE_OK = 0;
    
    /**
     * Validation will happen on the host. Any fields with the "Autopop" attribute
     * will have their values set by the server and returned to the client. The
     * record in the database is not updated. This gives the client the opportunity
     * to get server populated data inserted into the record and returned to the client
     * without updating the database.
     */
    static const int AUTO_POPULATE = 1;
    
    /**
     * The server validates the record and returns any errors found. It does not
     * update the database if there are no errors found.
     */
    static const int VALIDATE_ONLY = 2;
    
    /**
     * Returns the current value of the validation flag for the current request.
     *
     * @return An integer representing the value of the validation flag.
     */
    const int GetValidateFlag();
    
    /**
     * Sets the validation flag for the current request.
     *
     * @param flag One of UPDATE_OK, AUTO_POPULATE or VALIDATE_ONLY.
     */
    void SetValidateFlag(int flag);
    
    /**
     * Returns a string vector containing the warning numbers.
     *
     * @return A StringVector containing the warning numbers represented as strings.
     */
    const StringVector GetAllWarnings();
    
    /**
     * Returns a string containing the user response for the given warning number.
     *
     * @param warningNumber An integer containing the warning number.
     * @return A string containing the user response for the given warning number.
     */
    const std::string GetWarningResponse(int warningNumber);

    /**
     * Returns a string containing the user response for the given warning number.
     *
     * @param warningNumber A string containing the warning number.
     * @return A string containing the user response for the given warning number.
     */
    const std::string GetWarningResponse(std::string warningNumber);
    
    /**
     * Set the warning number and response.
     * @param warningNumber An integer containing the warning number.
     * @param userResponse A string containing the user response.
     */
    void SetWarningResponse(int warningNumber, std::string userResponse);
    
    /**
     * Set the warning number and response.
     * @param warningNumber A string containing the warning number.
     * @param userResponse A string containing the user response.
     */
    void SetWarningResponse(std::string warningNumber, std::string userResponse);
    
  private:
    static const char * CLASS_PARAMETER;
    static const char * DELIMITER_PARAMETER;
    static const char * RECORD_PARAMETER;
    static const char * RESOURCE_PARAMETER;
    static const char * UPDATE_TYPE_PARAMETER;
    static const char * UPDATE_ACTION_PARAMETER;
    static const char * VALIDATE_PARAMETER;
    static const char * WARNING_RESPONSE_PARAMETER;
    
    std::string     mDelimiter;
    StringMap       mFields;
    int             mFlag;
    std::string     mUpdateType;
    std::string     mAction;
    StringMap       mWarnings;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

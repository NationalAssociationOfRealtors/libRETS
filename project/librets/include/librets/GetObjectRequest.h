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

#ifndef LIBRETS_GET_OBJECT_REQUEST_H
#define LIBRETS_GET_OBJECT_REQUEST_H

/** 
 * @file GetObjectRequest.h
 * Contains the GetObjectRequest class definition.
 */

#include "librets/RetsObject.h"
#include "librets/http_forward.h"
#include "librets/std_forward.h"

namespace librets {

/**
 * A GetObjectRequest represents the parameters needed to perform a
 * RETS GetObject transaction.  A server does not have to provide
 * objects for all well-known types, but a server should use well-known
 * types, where appropriate.  Multiple objects for multiple
 * resource entities may be retrieved in a single request.
 */
class GetObjectRequest : public RetsObject
{
  public:
    /** The image well-known type. */
    static const char * PHOTO_TYPE;
    /** The property boundary well-known type. */
    static const char * PLAT_TYPE;
    /** The video well-known type. */
    static const char * VIDEO_TYPE;
    /** The audio well-known type. */
    static const char * AUDIO_TYPE;
    /** The thubmail image well-known type. */
    static const char * THUMBNAIL_TYPE;
    /** The location image well-known type. */
    static const char * MAP_TYPE;
    /** The muliple-view, interactive well-known type.  */
    static const char * VRIMAGE_TYPE;
    
    /**
     * Creates a new GetObjectRequest.  The type may be a well-known
     * type.
     *
     * @param resource A valid resource as defined in the metadata
     * @param type A valid type as defined in the metadata
     */
    GetObjectRequest(std::string resource, std::string type);
    
    virtual ~GetObjectRequest();
    
    /**
     * Returns if location URLs will be requested.
     *
     * @return <code>true</code> if location URLs will be requested.
     */
    bool GetLocation() const;
    
    /**
     * If set to <code>true</code>, the object data will be requested
     * as URLs instead of the actual data.  Servers are not required to
     * provide URLs for all objects, so a server may choose to ignore
     * this request.  By default, this is set to <code>false</code>.
     *
     * @param location <code>true</code> if location URLs are requested
     */
    void SetLocation(bool location);
    
    /**
     * Add a single object for a resource entity to the request.  This
     * should be used, for example, to request a single photo for a
     * property listing.  Object IDs are always sequent, beginning with 1.
     *
     * @param resourceEntity A value from the key field of the resource
     * @param objectId The particular object to be retrieved
     */
    void AddObject(std::string resourceEntity, int objectId);
    
    /**
     * Adds all objects for a resource entity to the request.  This
     * should be used, for example, to request all photos for a
     * property listing.
     *
     * @param resourceEntity A value for the key field of the resource
     */
    void AddAllObjects(std::string resourceEntity);
    
    /// @cond MAINTAINER
    /**
     * (Internal) Create the Http Request object that will control access
     * to the server to fetch the objects.
     */
    RetsHttpRequestPtr CreateHttpRequest() const;
    
    /**
     * (Internal) Set up the Http Request. This should only be called by
     * CreateHttpRequest.
     * @param httpRequest A pointer to the RetsHttpRequest.
     */
    void PrepareHttpRequest(RetsHttpRequestPtr httpRequest) const;
    /// @endcond
    
    /**
     * Returns <code>true</code> if a default object key and ID are
     * available.  A default object key and ID are availalbe if
     * and only if a single ojbect has been added.  With more than
     * one object added, there is no way to determine which is the default.
     *
     * @return <code>true</code> if a default object key and ID are available.
     */
    bool HasDefaultObjectKeyAndId() const;
    
    /**
     * Returns the default object key.
     * @return A string containing the default object key.
     */
    std::string GetDefaultObjectKey() const;
    
    /**
     * Returns the default object ID.
     * @return An integer containing the default object ID.
     */
    int GetDefaultObjectId() const;

    /**
     * Certain RETS servers have been known to violate the RETS spec
     * by allowing Content-Description fields with CRLF in it when
     * doing a multi-part response.  By the RETS spec, this is
     * actually creating a new header.  This causes an error when we
     * parse as having malformed headers.  By turning this on, you are
     * saying you know that their are malformed headers, but you do
     * not care.  Do NOT turn this on unless you know what you are
     * doing.  It is NOT recommended that this be used.  This only
     * effects multi-part responses.  The default is
     * <code>false</code>.
     * @param ignore A boolean that when <code>true</code> indicates that
     * malformed headers should be ignored.
     */
    void SetIgnoreMalformedHeaders(bool ignore);

    /// @cond MAINTAINER
    /**
     * Returns if the GetObject parsing should ignore malformed headers.
     *
     * @return <code>true</code> if malformed headers will be ignored.
     */
    bool GetIgnoreMalformedHeaders() const;
    /// @endcond
    
  private:
    typedef std::map<std::string, StringVectorPtr> ObjectList;
    
    StringVectorPtr GetObjectIds(std::string resourceEntity);
    
    void UpdateDefaultObjectKeyAndId();
    
    std::string mResource;
    std::string mType;
    bool mLocation;
    ObjectList mObjectList;
    bool mHasDefaultObjectKeyAndId;
    std::string mDefaultObjectKey;
    int mDefaultObjectId;
    bool mIgnoreMalformedHeaders;
};
    
}

#endif

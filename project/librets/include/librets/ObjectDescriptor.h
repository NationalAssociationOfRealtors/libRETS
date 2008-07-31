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

#ifndef LIBRETS_OBJECT_DESCRIPTOR_H
#define LIBRETS_OBJECT_DESCRIPTOR_H
/**
 * @file ObjectDescriptor.h 
 * Contains classes used to fetch media from the RETS server.
 */

#include <string>
#include "librets/RetsObject.h"
#include "librets/std_forward.h"

namespace librets {
	
/**
 * A helper class that wraps the media.
 * The BinaryData class wraps the media returned from the RETS server
 * into a string.
 */
class BinaryData
{
  public:
	/**
     * Report the size of the media object.
     * @return int representing the size in bytes of the media object.
     */
    int Size() const;
	/**
     * Obtain the media as a string.
     * @return std::string representing the media object.
     */
    std::string AsString() const;
	/**
     * Obtain the media as an array of characters.
     * @return pointer to the media.
     */
    const char * AsChar() const;
	/**
     * Make a copy of the data as the media object.
     * @param buffer[] An array of characters containing the media object.
     * @param length An int representing the length of the media object in bytes.
     */
    void Copy(unsigned char buffer[], int length) const; 
	/**
     * Load the data contained in the input stream as the media object.
     * @param inputStream A pointer to the input stream.
     */
    void ReadToEof(istreamPtr inputStream);
	
  private:
	std::string mData;
};

typedef std::auto_ptr<BinaryData> BinaryDataAPtr;

/**
 * An ObjectDescriptor represents a single object retrieved from
 * a GetObject transaction.
 */
class ObjectDescriptor : public RetsObject
{
  public:
    /**
     * Returns the object key for this object.  It should match
     * one of the resource entities in the request.
     *
     * @return The object key for this object
     */
    std::string GetObjectKey() const;
    
    /**
     * Sets the object key for this object.
     *
     * @param objectKey An object key
     */
    void SetObjectKey(std::string objectKey);
    
    /**
     * Returns the object ID for this object.
     *
     * @return The object ID for this object
     */
    int GetObjectId() const;
    
    /**
     * Sets the object ID for this object.
     *
     * @param objectId An object ID
     */
    void SetObjectId(int objectId);
    
    /**
     * Returns the description of this object.  If no description was
     * provided by the server, an empty string will be returned.
     *
     * @return The description of this object
     */
    std::string GetDescription() const;
    
    /**
     * Sets the description of this object.
     *
     * @param description A description
     */
    void SetDescription(std::string description);

    /**
     * Returns the location URL for this object.  This returns a non-empty
     * string if and only if location URLs were requested and the server
     * provides location URLs.  A server is not requered to provide
     * location URLs, even if it was requested.
     *
     * @return the location URL for this object
     */
    std::string GetLocationUrl() const;	
    
    /**
     * Sets the location URL for this object.
     *
     * @param locationUrl A location URL
     */
    void SetLocationUrl(std::string locationUrl);
    
    /**
     * Returns the MIME content type of this object, for example,
     * "image/jpeg".
     *
     * @return the MIME content type of this object.
     */
    std::string GetContentType() const;
    
    /**
     * Sets the MIME content type of this object.
     *
     * @param contentType A MIME content type
     */
    void SetContentType(std::string contentType);
    
    /**
     * Returns an input stream to the object data.  This object
     * is owned by this ObjectDescriptor and does not need to
     * be deleted.
     *
     * @return An input stream to the object data
     */
    istreamPtr GetDataStream();
    
    /**
     * Sets the input stream to the object data.
     *
     * @param dataStream A shared pointer to an input stream
     */
    void SetDataStream(istreamPtr dataStream);
	
    /**
     * Returns all the object data as a class by reading the input
     * stream until EOF is reached.  The returned object is not
     * owned by the ObjectDescriptor and must be deleted after use.
     * This method is not of much use to native C++ users, but aids
     * in providing wrappers to other language bindings.
     *
     * @return All the object data
     */
	BinaryDataAPtr GetData();
    
  private:
    std::string mObjectKey;
    int mObjectId;
    std::string mDescription;
    std::string mLocationUrl;
    std::string mContentType;
    istreamPtr mDataStream;
};
    
}

#endif

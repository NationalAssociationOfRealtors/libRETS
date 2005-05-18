#ifndef LIBRETS_OBJECT_DESCRIPTOR_H
#define LIBRETS_OBJECT_DESCRIPTOR_H

#include <string>
#include "librets/RetsObject.h"
#include "librets/std_forward.h"

namespace librets {

class ObjectDescriptor : public RetsObject
{
  public:
    
    std::string GetObjectKey() const;
    
    void SetObjectKey(std::string objectKey);
    
    int GetObjectId() const;
    
    void SetObjectId(int objectId);
    
    std::string GetDescription() const;
    
    void SetDescription(std::string description);

    std::string GetLocationUrl() const;	
    
    void SetLocationUrl(std::string locationUrl);
    
    std::string GetContentType() const;
    
    void SetContentType(std::string contentType);
    
    istreamPtr GetData();
    
    void SetData(istreamPtr data);
    
  private:
    std::string mObjectKey;
    int mObjectId;
    std::string mDescription;
    std::string mLocationUrl;
    std::string mContentType;
    istreamPtr mData;
};
    
}

#endif

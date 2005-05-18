#ifndef LIBRETS_GET_OBJECT_REQUEST_H
#define LIBRETS_GET_OBJECT_REQUEST_H

#include "librets/RetsObject.h"
#include "librets/http_forward.h"
#include "librets/std_forward.h"

namespace librets {
    
class GetObjectRequest : public RetsObject
{
  public:
    GetObjectRequest(std::string resource, std::string type);
    
    virtual ~GetObjectRequest();
    
    bool GetLocation() const;

    void SetLocation(bool location);
    
    void AddObject(std::string resourceEntity, int objectId);
    
    void AddAllObjects(std::string resourceEntity);
    
    RetsHttpRequestPtr CreateHttpRequest() const;
    
    void PrepareHttpRequest(RetsHttpRequestPtr httpRequest) const;
    
  private:
    typedef std::map<std::string, StringVectorPtr> ObjectList;
    
    StringVectorPtr GetObjectIds(std::string resourceEntity);
    
    std::string mResource;
    std::string mType;
    bool mLocation;
    ObjectList mObjectList;
};
    
}

#endif

#ifndef LIBRETS_GET_OBJECT_RESPONSE_H
#define LIBRETS_GET_OBJECT_RESPONSE_H

#include <vector>
#include "librets/RetsObject.h"
#include "librets/http_forward.h"
#include "librets/protocol_forward.h"
#include "librets/std_forward.h"

namespace librets {

class GetObjectResponse : public RetsObject
{
  public:
    GetObjectResponse();
    
    virtual ~GetObjectResponse();
    
    void Parse(RetsHttpResponsePtr httpResponse);
    
    ObjectDescriptorPtr NextObject();
    
  private:
    typedef std::vector<ObjectDescriptorPtr> ObjectList;
    
    void ParseSinglePart(RetsHttpResponsePtr httpResponse);
    void ParseMultiPart(RetsHttpResponsePtr httpResponse);
    
    std::string FindBoundary(std::string contentType);
    void ParsePartStream(istreamPtr in);
    
    ObjectList mObjects;
    ObjectList::iterator mNextObject;
};
    
}

#endif

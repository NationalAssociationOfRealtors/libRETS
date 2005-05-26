#ifndef LIBRETS_GET_OBJECT_QUERY_H
#define LIBRETS_GET_OBJECT_QUERY_H

#include "librets/std_forward.h"
#include "librets/sql_forward.h"
#include "librets/RetsObject.h"

namespace librets {
    
class GetObjectQuery : public RetsObject
{
  public:
    GetObjectQuery();
    
    std::string GetResource() const;
    
    void SetResource(std::string resource);
    
    std::string GetType() const;
    
    void SetType(std::string type);
    
    std::string GetObjectKey() const;
    
    void SetObjectKey(std::string objectKey);
    
    IntVectorPtr GetObjectIds() const;
    
    void AddObjectId(int objectId);
    
    virtual std::ostream & Print(std::ostream & outputStream) const;

    
  private:
    IntVectorPtr mObjectIds;
    std::string mResource;
    std::string mType;
    std::string mObjectKey;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

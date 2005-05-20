#ifndef LIBRETS_METADATA_BY_LEVEL_COLLECTOR_H
#define LIBRETS_METADATA_BY_LEVEL_COLLECTOR_H

#include <map>
#include "librets/RetsObject.h"
#include "librets/MetadataElementCollector.h"
#include "librets/MetadataElement.h"

namespace librets {

class MetadataByLevelCollector :
    public virtual RetsObject, public MetadataElementCollector
{
  public:
    MetadataByLevelCollector();

    virtual ~MetadataByLevelCollector();

    void AddElement(MetadataElementPtr element);
    
    MetadataElementListPtr Find(MetadataElement::MetadataType type,
                                std::string level);

  private:
    typedef std::map<std::string, MetadataElementListPtr> LevelMap;

    typedef boost::shared_ptr<LevelMap> LevelMapPtr;

    typedef std::map<MetadataElement::MetadataType, LevelMapPtr>
        TypeMap;

    TypeMap mTypeMap;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

#ifndef LIBRETS_METADATA_SEARCH_HELP_H
#define LIBRETS_METADATA_SEARCH_HELP_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Search help metadata.
 */
class MetadataSearchHelp : public MetadataElement
{
  public:
    /**
     * Always returns SEARCH_HELP.
     *
     * @return SEARCH_HELP
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

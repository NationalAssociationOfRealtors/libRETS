#ifndef LIBRETS_METADATA_UPDATE_HELP_H
#define LIBRETS_METADATA_UPDATE_HELP_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Update help metadata.
 */
class MetadataUpdateHelp : public MetadataElement
{
  public:
    /**
     * Always returns UPDATE_HELP.
     *
     * @return UPDATE_HELP
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

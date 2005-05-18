#ifndef LIBRETS_METADATA_FOREIGN_KEY_H
#define LIBRETS_METADATA_FOREIGN_KEY_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Foreign key metadata.
 */
class MetadataForeignKey : public MetadataElement
{
  public:
    /**
     * Always returns FOREIGN_KEY.
     *
     * @return FOREIGN_KEY
     */
    MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

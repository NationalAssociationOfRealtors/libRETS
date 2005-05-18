#ifndef LIBRETS_METADATA_VALIDATION_EXPRESSION_H
#define LIBRETS_METADATA_VALIDATION_EXPRESSION_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Validation expression metadata.
 */
class MetadataValidationExpression : public MetadataElement
{
  public:
    /**
     * Always returns VALIDATION_EXPRESSION
     *
     * @return VALIDATION_EXPRESSION
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

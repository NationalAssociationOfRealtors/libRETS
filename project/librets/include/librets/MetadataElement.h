#ifndef LIBRETS_METADATA_ELEMENT_H
#define LIBRETS_METADATA_ELEMENT_H

#include <string>
#include <vector>
#include "librets/forward.h"

namespace librets {

/**
 * A metadata element.
 */
class MetadataElement
{
  public:

    /**
     * Metadata types.
     */
    enum MetadataType
    {
        /** System metadata. */
        SYSTEM,
        /** Resource metadata. */
        RESOURCE,
        /** Class metadata. */
        CLASS,
        /** Table metadata. */
        TABLE,
        /** Update metadata. */
        UPDATE,
        /** Update type metadata. */
        UPDATE_TYPE,
        /** Object metadata. */
        OBJECT,
        /** Search help metadata. */
        SEARCH_HELP,
        /** Edit mask metadata. */
        EDIT_MASK,
        /** Lookup metadata. */
        LOOKUP,
        /** Lookup type metadata. */
        LOOKUP_TYPE,
        /** Update help metadata. */
        UPDATE_HELP,
        /** Validation lookup metadata. */
        VALIDATION_LOOKUP,
        /** Validation lookup type metadata. */
        VALIDATION_LOOKUP_TYPE,
        /** Validation external metadata. */
        VALIDATION_EXTERNAL,
        /** Validation external type metadata. */
        VALIDATION_EXTERNAL_TYPE,
        /** Validation expression metadata. */
        VALIDATION_EXPRESSION,
        /** Foreign key metadata. */
        FOREIGN_KEY
    };

    /**
     * Returns the type of metadata element.
     *
     * @return the type of metadata element
     */
    virtual MetadataType GetType() const = 0;

    /**
     * Returns the value of an attribute.
     *
     * @param attributeName the name of an attribute
     * @return the value of the attribute
     */
    std::string GetAttribute(std::string attributeName) const;

    /**
     * Returns the children of this element matching the given type.
     *
     * @param type The type of children to retrieve
     * @return a vector of children
     */
    std::vector<MetadataElementPtr> GetChildren(MetadataType type);
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

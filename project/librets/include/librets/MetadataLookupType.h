/*
 * Copyright (C) 2005 National Association of REALTORS(R)
 *
 * All rights reserved.
 *
 * Permission is hereby granted, free of charge, to any person
 * obtaining a copy of this software and associated documentation
 * files (the "Software"), to deal in the Software without
 * restriction, including without limitation the rights to use, copy,
 * modify, merge, publish, distribute, and/or sell copies of the
 * Software, and to permit persons to whom the Software is furnished
 * to do so, provided that the above copyright notice(s) and this
 * permission notice appear in all copies of the Software and that
 * both the above copyright notice(s) and this permission notice
 * appear in supporting documentation.
 */
#ifndef LIBRETS_METADATA_LOOKUP_TYPE_H
#define LIBRETS_METADATA_LOOKUP_TYPE_H

#include "librets/MetadataElement.h"

namespace librets {

/**
 * Lookup type metadata.
 */
class MetadataLookupType : public MetadataElement
{
  public:
    /**
     * Always returns LOOKUP_TYPE.
     *
     * @return LOOKUP_TYPE
     */
    virtual MetadataType GetType() const;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

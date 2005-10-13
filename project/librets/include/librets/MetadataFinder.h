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

#ifndef LIBRETS_METADATA_FINDER_H
#define LIBRETS_METADATA_FINDER_H

#include "librets/metadata_forward.h"
#include "librets/MetadataElement.h"

namespace librets {
    
class MetadataFinder
{
  public:
    virtual ~MetadataFinder();
    
    virtual MetadataElementListPtr FindByLevel(MetadataElement::Type type,
                                               std::string level) = 0;
    
    virtual MetadataElementPtr FindByPath(MetadataElement::Type type,
                                          std::string path) = 0;
};

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

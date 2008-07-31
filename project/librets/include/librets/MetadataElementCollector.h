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

#ifndef LIBRETS_METADATA_ELEMENT_COLLECTOR_H
#define LIBRETS_METADATA_ELEMENT_COLLECTOR_H
/** 
 * @file MetadataElementCollector.h
 * Contains the MetadataElementCollector class declaration.
 */
/// @cond MAINTAINER

#include "librets/metadata_forward.h"

namespace librets {

/**
 * (Internal) Contains the prototype for the MetadataElementCollector
 * from which other classes inherit.
 */
class MetadataElementCollector
{
  public:
    virtual ~MetadataElementCollector();

    /**
     * Add the element to the list of elements.
     * @param element A pointer to the MetadataElement to be added to the
     * list.
     */
    virtual void AddElement(MetadataElementPtr element) = 0;
};

};

/// @endcond
#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

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

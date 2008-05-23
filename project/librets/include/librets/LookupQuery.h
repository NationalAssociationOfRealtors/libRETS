/*
 * Copyright (C) 2008 National Association of REALTORS(R)
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

#ifndef LIBRETS_LOOKUP_QUERY_H
#define LIBRETS_LOOKUP_QUERY_H

#include "RetsObject.h"

namespace librets {

class LookupQuery : public RetsObject
{
  public:
    LookupQuery();
    virtual ~LookupQuery();

    std::string GetResource() const;

    void SetResource(std::string resource);

    std::string GetLookup() const;

    void SetLookup(std::string lookup);

  private:
    std::string mResource;
    std::string mLookup;
};

};

#endif /* LIBRETS_LOOKUP_QUERY_H */

/* Local Variables: */
/* mode: c++ */
/* End: */


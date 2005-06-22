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
#ifndef LIBRETS_TEST_OBJECT_H
#define LIBRETS_TEST_OBJECT_H

#include <boost/shared_ptr.hpp>
#include "librets/RetsObject.h"

namespace librets {

class TestObject : public RetsObject
{
  public:
    TestObject(int number) : mNumber(number) { };
    virtual ~TestObject();
    virtual std::ostream & Print(std::ostream & s) const;
    virtual bool Equals(const RetsObject * rhs) const;

  private:
    int mNumber;
};

typedef boost::shared_ptr<TestObject> TestObjectPtr;

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

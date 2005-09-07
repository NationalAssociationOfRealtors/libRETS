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

#include <cppunit/extensions/HelperMacros.h>
#include "testUtil.h"
#include "librets/sql_forward.h"
#include "librets/LookupOrCriterion.h"
#include "librets/DmqlExpression.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS LookupOrCriterionTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testToDmql);
    CPPUNIT_TEST(testCoalescing);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testEquals();
    void testToDmql();
    void testCoalescing();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testEquals()
{
    DmqlCriterionPtr foo = literal("foo");
    DmqlCriterionPtr bar = literal("bar");
    LookupOrCriterion lookupOr1("field1", foo);
    LookupOrCriterion lookupOr2("field1", foo);
    LookupOrCriterion lookupOr3("field1", bar);
    LookupOrCriterion lookupOr4("field2", foo);
    LookupOrCriterion lookupOr5("field2", bar);

    ASSERT_EQUAL(lookupOr1, lookupOr2);
    ASSERT_NOT_EQUAL(lookupOr1, lookupOr3);
    ASSERT_NOT_EQUAL(lookupOr1, lookupOr4);
    ASSERT_NOT_EQUAL(lookupOr1, lookupOr5);
    ASSERT_NOT_EQUAL(lookupOr3, lookupOr4);
    ASSERT_NOT_EQUAL(lookupOr3, lookupOr5);
}

void CLASS::testToDmql()
{
    LookupOrCriterion lookupOr("field", literal("foo"));
    ASSERT_STRING_EQUAL("(field=|foo)", lookupOr.ToDmqlString());
    lookupOr.add(literal("bar"));
    ASSERT_STRING_EQUAL("(field=|foo,bar)", lookupOr.ToDmqlString());
}

void CLASS::testCoalescing()
{
#if 0
    DmqlCriterionPtr foo = literal("foo");
    DmqlCriterionPtr bar = literal("bar");
    
    // Lookups on the same field should coalesce
    LookupOrCriterionPtr c1(new LookupOrCriterion("field", foo));
    c1->add(bar);
    DmqlCriterionPtr c2 = lookupOr("field", foo, lookupOr("field", bar))
    ASSERT_EQUAL(*DmqlCriterionPtr(c1), *c2);
    
    // Lookups on different fields should not
    c2 = lookupOr("field", foo, lookupOr("field2", bar));
    ASSERT_NOT_EQUAL(*DmqlCriterionPtr(c1), *c2);
#endif
}

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
#include "librets/DmqlExpression.h"
#include "librets/LiteralCriterion.h"
#include "librets/EqCriterion.h"
#include "librets/GtCriterion.h"
#include "librets/LtCriterion.h"
#include "librets/OrCriterion.h"
#include "librets/AndCriterion.h"
#include "librets/LookupOrCriterion.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS DmqlExpressionTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testLiteralString);
    CPPUNIT_TEST(testLiteralInt);
    CPPUNIT_TEST(testEq);
    CPPUNIT_TEST(testGt);
    CPPUNIT_TEST(testLt);
    CPPUNIT_TEST(testLogicOr);
    CPPUNIT_TEST(testOrCoalescing);
    CPPUNIT_TEST(testAndCoalescing);
    CPPUNIT_TEST(testLookupOrCoalescing);
    CPPUNIT_TEST_SUITE_END();
    
  public:
    void setUp();
    
  protected:
    void testLiteralString();
    void testLiteralInt();
    void testEq();
    void testGt();
    void testLt();
    void testLogicOr();
    void testOrCoalescing();
    void testAndCoalescing();
    void testLookupOrCoalescing();
    
    DmqlCriterionPtr mFoo;
    DmqlCriterionPtr mBar;
    DmqlCriterionPtr mBaz;
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::setUp()
{
    mFoo = literal("foo");
    mBar = literal("bar");
    mBaz = literal("baz");
}

void CLASS::testLiteralString()
{
    LiteralCriterionPtr c2(new LiteralCriterion("foo"));
    ASSERT_EQUAL(*DmqlCriterionPtr(c2), *mFoo);
}

void CLASS::testLiteralInt()
{
    DmqlCriterionPtr c1 = literal(1);
    LiteralCriterionPtr c2(new LiteralCriterion("1"));
    ASSERT_EQUAL(*DmqlCriterionPtr(c2), *c1);
}

void CLASS::testEq()
{
    DmqlCriterionPtr c1 = eq("field", mFoo);
    EqCriterionPtr c2(new EqCriterion("field", mFoo));
    ASSERT_EQUAL(*DmqlCriterionPtr(c2), *c1);
}

void CLASS::testGt()
{
    DmqlCriterionPtr c1 = gt("field", mFoo);
    GtCriterionPtr c2(new GtCriterion("field", mFoo));
    ASSERT_EQUAL(*DmqlCriterionPtr(c2), *c1);
}

void CLASS::testLt()
{
    DmqlCriterionPtr c1 = lt("field", mFoo);
    LtCriterionPtr c2(new LtCriterion("field", mFoo));
    ASSERT_EQUAL(*DmqlCriterionPtr(c2), *c1);
}

void CLASS::testLogicOr()
{
    DmqlCriterionPtr c1 = logicOr(mFoo, mBar);
    OrCriterionPtr c2(new OrCriterion(mFoo, mBar));
    ASSERT_EQUAL(*DmqlCriterionPtr(c2), *c1);
}

void CLASS::testOrCoalescing()
{
    // (foo + bar) + baz should equal (foo + bar + baz)
    OrCriterionPtr c1(new OrCriterion(mFoo, mBar));
    c1->add(mBaz);
    DmqlCriterionPtr c2 = logicOr(mFoo, logicOr(mBar, mBaz));
    ASSERT_EQUAL(*DmqlCriterionPtr(c1), *c2);
    
    // (foo + bar) * baz should not equal (foo + bar + baz)
    c2 = logicOr(mFoo, logicAnd(mBar, mBaz));
    ASSERT_NOT_EQUAL(*DmqlCriterionPtr(c1), *c2);
}

void CLASS::testAndCoalescing()
{
    // foo * (bar * baz) should equal (foo * bar * baz)
    AndCriterionPtr c1(new AndCriterion(mFoo, mBar));
    c1->add(mBaz);
    DmqlCriterionPtr c2 = logicAnd(mFoo, logicAnd(mBar, mBaz));
    ASSERT_EQUAL(*DmqlCriterionPtr(c1), *c2);
    
    // foo * (bar + baz) should not equal (foo * bar * baz)
    c2 = logicAnd(mFoo, logicOr(mBar, mBaz));
    ASSERT_NOT_EQUAL(*DmqlCriterionPtr(c1), *c2);
}

void CLASS::testLookupOrCoalescing()
{
    // Lookup ors with the same field should coalesce
    LookupOrCriterionPtr c1(new LookupOrCriterion("field", mFoo));
    c1->add(mBar);
    
    DmqlCriterionPtr c2 = logicOr(lookupOr("field", mFoo),
                                  lookupOr("field", mBar));
    ASSERT_EQUAL(*DmqlCriterionPtr(c1), *c2);
    
    // Lookup ors with different fields should not coalesce
    c2 = logicOr(lookupOr("field", mFoo), lookupOr("field2", mBar));
    ASSERT_NOT_EQUAL(*DmqlCriterionPtr(c1), *c2);
}

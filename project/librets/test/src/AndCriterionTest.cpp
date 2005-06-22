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
#include "librets/AndCriterion.h"
#include "librets/LiteralCriterion.h"
#include "librets/DmqlExpression.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS AndCriterionTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testToDmql);
    CPPUNIT_TEST(testDmqlExpression);
    CPPUNIT_TEST(testAssociativeMerging);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testEquals();
    void testToDmql();
    void testDmqlExpression();
    void testAssociativeMerging();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testEquals()
{
    DmqlCriterionPtr foo = literal("foo");
    DmqlCriterionPtr bar = literal("bar");
    DmqlCriterionPtr field1 = literal("field1");
    DmqlCriterionPtr field2 = literal("field2");
    AndCriterion and1(field1, foo);
    AndCriterion and2(field1, foo);
    AndCriterion and3(field1, bar);
    AndCriterion and4(field2, foo);
    AndCriterion and5(field2, bar);

    ASSERT_EQUAL(and1, and2);
    ASSERT_NOT_EQUAL(and1, and3);
    ASSERT_NOT_EQUAL(and1, and4);
    ASSERT_NOT_EQUAL(and1, and5);
    ASSERT_NOT_EQUAL(and3, and4);
    ASSERT_NOT_EQUAL(and3, and5);
}

void CLASS::testToDmql()
{
    AndCriterion criterion(literal("foo"), literal("bar"));
    ASSERT_STRING_EQUAL("(foo,bar)", criterion.ToDmqlString());
}

void CLASS::testDmqlExpression()
{
    DmqlCriterionPtr c1 = logicAnd(literal("field1"), literal("1"));
    DmqlCriterionPtr c2 = logicAnd(literal("field1"), literal(1));
    DmqlCriterionPtr c3 = logicAnd(literal("field1"), literal("2"));

    ASSERT_EQUAL(*c1, *c2);
    ASSERT_NOT_EQUAL(*c1, *c3);
}

void CLASS::testAssociativeMerging()
{
    DmqlCriterionPtr foo = literal("foo");
    DmqlCriterionPtr bar = literal("bar");
    DmqlCriterionPtr baz = literal("baz");
    // foo * (bar * baz) should equal (foo * bar * baz)
    AndCriterionPtr expected(new AndCriterion(foo, bar));
    expected->add(baz);
    DmqlCriterionPtr actual = logicAnd(foo, logicAnd(bar, baz));
    ASSERT_EQUAL(*DmqlCriterionPtr(expected), *actual);

    // foo * (bar + baz) should not equal (foo * bar * baz)
    actual = logicAnd(foo, logicOr(bar, baz));
    ASSERT_NOT_EQUAL(*DmqlCriterionPtr(expected), *actual);
}

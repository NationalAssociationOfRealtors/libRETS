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
#include "librets/LtCriterion.h"
#include "librets/DmqlExpression.h"

using namespace librets;
using namespace librets::DmqlExpression;
using namespace std;

#define CLASS LtCriterionTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST(testToDmql);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testEquals();
    void testToDmql();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testEquals()
{
    DmqlCriterionPtr foo = literal("foo");
    DmqlCriterionPtr bar = literal("bar");
    LtCriterion lt1("field1", foo);
    LtCriterion lt2("field1", foo);
    LtCriterion lt3("field1", bar);
    LtCriterion lt4("field2", foo);
    LtCriterion lt5("field2", bar);

    ASSERT_EQUAL(lt1, lt2);
    ASSERT_NOT_EQUAL(lt1, lt3);
    ASSERT_NOT_EQUAL(lt1, lt4);
    ASSERT_NOT_EQUAL(lt1, lt5);
    ASSERT_NOT_EQUAL(lt3, lt4);
    ASSERT_NOT_EQUAL(lt3, lt5);
}

void CLASS::testToDmql()
{
    LtCriterion lt("field", literal(1));
    ASSERT_STRING_EQUAL("(field=1-)", lt.ToDmqlString());
}

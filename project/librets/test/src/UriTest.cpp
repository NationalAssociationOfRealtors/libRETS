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
#include <sstream>
#include "testUtil.h"
#include "librets/Uri.h"

using namespace librets;
using namespace std;

#define CLASS UriTest

class CLASS : public CPPUNIT_NS::TestFixture
{
    CPPUNIT_TEST_SUITE(CLASS);
    CPPUNIT_TEST(testParseNoPath);
    CPPUNIT_TEST(testParsePath);
    CPPUNIT_TEST(testParsePort);
    CPPUNIT_TEST(testRelative);
    CPPUNIT_TEST(testResolve);
    CPPUNIT_TEST(testPrint);
    CPPUNIT_TEST(testEquals);
    CPPUNIT_TEST_SUITE_END();

  protected:
    void testParseNoPath();
    void testParsePath();
    void testParsePort();
    void testRelative();
    void testResolve();
    void testPrint();
    void testEquals();
};

CPPUNIT_TEST_SUITE_REGISTRATION(CLASS);

void CLASS::testParseNoPath()
{
    Uri uri("http://example.com/");
    CPPUNIT_ASSERT(uri.IsAbsolute());
    ASSERT_STRING_EQUAL("http", uri.GetScheme());
    ASSERT_STRING_EQUAL("example.com", uri.GetHost());
    CPPUNIT_ASSERT_EQUAL(80, uri.GetPort());
    ASSERT_STRING_EQUAL("/", uri.GetPath());
}

void CLASS::testParsePath()
{
    Uri uri("http://example.com/subdir/file.html");
    CPPUNIT_ASSERT(uri.IsAbsolute());
    ASSERT_STRING_EQUAL("http", uri.GetScheme());
    ASSERT_STRING_EQUAL("example.com", uri.GetHost());
    CPPUNIT_ASSERT_EQUAL(80, uri.GetPort());
    ASSERT_STRING_EQUAL("/subdir/file.html", uri.GetPath());
}

void CLASS::testParsePort()
{
    Uri uri("http://example.com:6103/rets/login");
    CPPUNIT_ASSERT(uri.IsAbsolute());
    ASSERT_STRING_EQUAL("http", uri.GetScheme());
    ASSERT_STRING_EQUAL("example.com", uri.GetHost());
    CPPUNIT_ASSERT_EQUAL(6103, uri.GetPort());
    ASSERT_STRING_EQUAL("/rets/login", uri.GetPath());
}

void CLASS::testRelative()
{
    Uri uri1("/rets/login");
    CPPUNIT_ASSERT(!uri1.IsAbsolute());
    ASSERT_STRING_EQUAL("/rets/login", uri1.GetPath());

    Uri uri2("rets/login");
    CPPUNIT_ASSERT(!uri2.IsAbsolute());
    ASSERT_STRING_EQUAL("rets/login", uri2.GetPath());

    Uri uri3("../login");
    CPPUNIT_ASSERT(!uri3.IsAbsolute());
    ASSERT_STRING_EQUAL("../login", uri3.GetPath());
}

void CLASS::testResolve()
{
    ASSERT_EQUAL(Uri("http://example.com/"),
                 Uri("http://example.com/", "http://example.com/rets/login"));
    ASSERT_EQUAL(Uri("http://example.com/file"),
                 Uri("/file", "http://example.com/rets/login"));
    ASSERT_EQUAL(Uri("http://example.com/rets/login/file"),
                 Uri("file", "http://example.com/rets/login/"));
    ASSERT_EQUAL(Uri("http://example.com/rets/file"),
                 Uri("file", "http://example.com/rets/login"));
    ASSERT_EQUAL(Uri("http://example.com/file"),
                 Uri("/file", "http://example.com/rets/login/"));
}

void CLASS::testPrint()
{
    Uri uri1("http://example.com/");
    Uri uri2("http://example.com:80/");
    Uri uri3("http://example.com:6103/rets/login");
    Uri uri4("/rets/login");

    ASSERT_STRING_EQUAL("http://example.com/", uri1.ToString());
    ASSERT_STRING_EQUAL("http://example.com/", uri2.ToString());
    ASSERT_STRING_EQUAL("http://example.com:6103/rets/login", uri3.ToString());
    ASSERT_STRING_EQUAL("/rets/login", uri4.ToString());
}

void CLASS::testEquals()
{
    Uri uri1("http://example.com/");
    Uri uri2("http://example.com:80/");
    Uri uri3("http://example.com:6103/rets/login");
    Uri uri4("/rets/login");

    ASSERT_EQUAL(uri1, uri2);
    ASSERT_NOT_EQUAL(uri1, uri3);
    ASSERT_NOT_EQUAL(uri1, uri4);
    ASSERT_NOT_EQUAL(uri2, uri3);
    ASSERT_NOT_EQUAL(uri2, uri4);
}

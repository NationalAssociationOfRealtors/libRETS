#ifndef LIBRETS_TEST_UTIL_H
#define LIBRETS_TEST_UTIL_H

#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <cppunit/extensions/HelperMacros.h>
#include "librets/util.h"

namespace librets {

void checkStringEquals(std::string expected, std::string actual,
                       CPPUNIT_NS::SourceLine sourceLine);

#define ASSERT_STRING_EQUAL(expected, actual) \
    checkStringEquals(expected, actual, CPPUNIT_SOURCELINE())

template<class T>
void checkNotEquals(const T& expected, const T& actual,
                    CPPUNIT_NS::SourceLine sourceLine)
{
    bool assertion = (expected != actual);
    if (!assertion)
    {
        CPPUNIT_NS::Asserter::failNotEqual(
            CPPUNIT_NS::assertion_traits<T>::toString(expected),
            CPPUNIT_NS::assertion_traits<T>::toString(actual),
            sourceLine);
    }
}

#define ASSERT_NOT_EQUAL(expected, actual) \
    checkNotEquals(expected, actual, CPPUNIT_SOURCELINE())


#define ASSERT_EQUAL CPPUNIT_ASSERT_EQUAL

template<class T>
void checkVectorEquals(const std::vector<T> & expected,
                       const std::vector<T> & actual,
                       CPPUNIT_NS::SourceLine sourceLine)
{
    if (!librets::util::VectorEquals(expected, actual))
    {
        std::ostringstream message;
        message << "Comparison failure: " << std::endl;
        message << "Expected: " << librets::util::Output(expected)
                << std::endl;
        message << "Actual  : " << librets::util::Output(actual);
        CPPUNIT_NS::Asserter::fail(
            CPPUNIT_NS::Message(message.str()), sourceLine);
    }
}

void checkVectorEquals(const std::vector<std::string> & expected,
                       const std::vector<std::string> & actual,
                       CPPUNIT_NS::SourceLine sourceLine);


#define ASSERT_VECTOR_EQUAL(expected, actual) \
    checkVectorEquals(expected, actual, CPPUNIT_SOURCELINE());

istreamPtr getResource(std::string resourceName);

};

#endif

/* Local Variables: */
/* mode: c++ */
/* End: */

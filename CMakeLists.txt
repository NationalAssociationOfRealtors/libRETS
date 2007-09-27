PROJECT(librets)

SET(LIBRETS_VERSION "1.1.8")
ADD_DEFINITIONS(-DLIBRETS_VERSION=\\\"${LIBRETS_VERSION}\\\")

# This is on until we learn more about cmake
SET(CMAKE_START_TEMP_FILE "" CACHE STRING "Foo")
SET(CMAKE_END_TEMP_FILE "" CACHE STRING "FOO")
SET(CMAKE_VERBOSE_MAKEFILE ON CACHE BOOL "Foo again")

# Basics of the build process we know we want.
SET(LIBRETS_BUILD_SQL_COMPILER OFF CACHE BOOL "Build the SQL2DMQL compiler?")
SET(LIBRETS_BUILD_EXAMPLES ON CACHE BOOL "Build the examples files?")

FIND_PACKAGE(Java)
FIND_PACKAGE(Doxygen)
FIND_PACKAGE(Boost)
FIND_PACKAGE(CURL)
FIND_PACKAGE(EXPAT)
FIND_PACKAGE(SWIG)

# Check to see if everything we need is present.
IF(NOT JAVA_RUNTIME)
  MESSAGE(FATAL_ERROR "Missing java runtime.")
ENDIF(NOT JAVA_RUNTIME)

IF(NOT Boost_FOUND)
# I'll want to add a version check at some point
  MESSAGE(FATAL_ERROR "Missing Boost")
ENDIF(NOT Boost_FOUND)

IF(NOT CURL_FOUND)
# I'll want to do a version check at some point.
  MESSAGE(FATAL_ERROR "Curl is missing")
ENDIF(NOT CURL_FOUND)

IF(NOT DOXYGEN_FOUND)
  MESSAGE(FATAL_ERROR "Missing Doxygen application.  Please edit DOXYGEN_EXECUTABLE with its path")
ENDIF(NOT DOXYGEN_FOUND)

IF(NOT SWIG_FOUND)
  MESSAGE(STATUS "SWIG wasn't found.  Not building bindings for other languages")
ENDIF(NOT SWIG_FOUND)

ADD_SUBDIRECTORY(project/librets/src ${CMAKE_BINARY_DIR}/build/librets)

IF (LIBRETS_BUILD_EXAMPLES)
  ADD_SUBDIRECTORY(project/examples/src ${CMAKE_BINARY_DIR}/build/examples)
ENDIF(LIBRETS_BUILD_EXAMPLES)

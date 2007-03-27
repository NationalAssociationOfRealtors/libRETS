INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/project/librets/include)

LINK_DIRECTORIES(${CMAKE_BINARY_DIR}/build/librets)
LINK_LIBRARIES(rets ${Boost_LIBRARY_DIR}/libboost_filesystem.a)

ADD_EXECUTABLE(xml xml.cpp)
TARGET_LINK_LIBRARIES(xml ${EXPAT_LIBRARY})

ADD_EXECUTABLE(http http.cpp)
TARGET_LINK_LIBRARIES(http ${CURL_LIBRARY})

ADD_EXECUTABLE(raw-rets raw-rets.cpp)
TARGET_LINK_LIBRARIES(raw-rets ${EXPAT_LIBRARY} ${CURL_LIBRARY})

ADD_EXECUTABLE(login login.cpp Options.cpp)
TARGET_LINK_LIBRARIES(login boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

ADD_EXECUTABLE(metadata metadata.cpp Options.cpp)
TARGET_LINK_LIBRARIES(metadata boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

ADD_EXECUTABLE(search search.cpp Options.cpp)
TARGET_LINK_LIBRARIES(search boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

ADD_EXECUTABLE(get-object get-object.cpp Options.cpp)
TARGET_LINK_LIBRARIES(get-object boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

IF (LIBRETS_BUILD_SQL_COMPILER AND ANTLR_FOUND)
  ADD_EXECUTABLE(sql2dmql sql2dmql.cpp)
ENDIF(LIBRETS_BUILD_SQL_COMPILER AND ANTLR_FOUND)

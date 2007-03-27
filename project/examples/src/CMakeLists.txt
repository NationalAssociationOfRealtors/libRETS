INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/project/librets/include ${Boost_INCLUDE_DIR} ${CURL_INCLUDE_DIR} ${EXPAT_INCLUDE_DIR})

INCLUDE(${CMAKE_SOURCE_DIR}/project/librets/src/sources.cmake)

SET(LR_LIBS ${EXPAT_LIBRARY} ${CURL_LIBRARY} boost_filesystem)

ADD_EXECUTABLE(xml xml.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(xml ${LR_LIBS})

ADD_EXECUTABLE(http http.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(http ${LR_LIBS})

ADD_EXECUTABLE(raw-rets raw-rets.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(raw-rets ${LR_LIBS})

ADD_EXECUTABLE(login login.cpp Options.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(login ${LR_LIBS} boost_program_options)

ADD_EXECUTABLE(metadata metadata.cpp Options.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(metadata ${LR_LIBS} boost_program_options)

ADD_EXECUTABLE(search search.cpp Options.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(search ${LR_LIBS} boost_program_options)

ADD_EXECUTABLE(get-object get-object.cpp Options.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(get-object ${LR_LIBS} boost_program_options)

IF (LIBRETS_BUILD_SQL_COMPILER AND ANTLR_FOUND)
  ADD_EXECUTABLE(sql2dmql sql2dmql.cpp)
ENDIF(LIBRETS_BUILD_SQL_COMPILER AND ANTLR_FOUND)

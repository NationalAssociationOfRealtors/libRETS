INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/project/librets/include)

INCLUDE(${CMAKE_SOURCE_DIR}/project/librets/src/sources.cmake)

SET(LR_LIBS ${EXPAT_LIBRARY} ${CURL_LIBRARY} boost_filesystem)

ADD_EXECUTABLE(xml xml.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(xml ${LR_LIBS})

ADD_EXECUTABLE(http http.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(http ${LR_LIBS})

ADD_EXECUTABLE(raw-rets raw-rets.cpp ${LIBRETS_SOURCES})
TARGET_LINK_LIBRARIES(raw-rets ${LR_LIBS})

# ADD_EXECUTABLE(login login.cpp Options.cpp)
# TARGET_LINK_LIBRARIES(login boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

# ADD_EXECUTABLE(metadata metadata.cpp Options.cpp)
# TARGET_LINK_LIBRARIES(metadata boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

# ADD_EXECUTABLE(search search.cpp Options.cpp)
# TARGET_LINK_LIBRARIES(search boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

# ADD_EXECUTABLE(get-object get-object.cpp Options.cpp)
# TARGET_LINK_LIBRARIES(get-object boost_program_options ${EXPAT_LIBRARY} ${CURL_LIBRARY})

# IF (LIBRETS_BUILD_SQL_COMPILER AND ANTLR_FOUND)
#   ADD_EXECUTABLE(sql2dmql sql2dmql.cpp)
# ENDIF(LIBRETS_BUILD_SQL_COMPILER AND ANTLR_FOUND)

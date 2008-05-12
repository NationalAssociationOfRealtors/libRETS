########################################################################
#
# librets
#

LIBRETS_ANTLR_GRAMMAR_DIR = $(top_srcdir)/project/librets/src
LIBRETS_ANTLR_PARSER = $(LIBRETS_ANTLR_GRAMMAR_DIR)/rets-sql.g
LIBRETS_ANTLR_TREE_PARSER = $(LIBRETS_ANTLR_GRAMMAR_DIR)/dmql-tree.g
LIBRETS_ANTLR_GET_OBJECT_TREE_PARSER = \
	$(LIBRETS_ANTLR_GRAMMAR_DIR)/get-object-tree.g

LIBRETS_ANTLR_GRAMMARS = $(LIBRETS_ANTLR_PARSER) $(LIBRETS_ANTLR_TREE_PARSER) \
	$(LIBRETS_ANTLR_GET_OBJECT_TREE_PARSER)
LIBRETS_ANTLR_SRC_DIR = build/librets/antlr
LIBRETS_ANTLR_OBJ_DIR = $(LIBRETS_ANTLR_SRC_DIR)
LIBRETS_ANTLR_TRIGGER = $(LIBRETS_ANTLR_SRC_DIR)/.antlr-up-to-date
LIBRETS_ANTLR_HDR_FILES = $(patsubst %, $(LIBRETS_ANTLR_SRC_DIR)/%, \
			RetsSqlLexer.hpp RetsSqlParser.hpp \
			DmqlTreeParser.hpp GetObjectTreeParser.hpp)
LIBRETS_ANTLR_SRC_FILES = $(patsubst %, $(LIBRETS_ANTLR_SRC_DIR)/%, \
			RetsSqlLexer.cpp RetsSqlParser.cpp \
			DmqlTreeParser.cpp GetObjectTreeParser.cpp)
LIBRETS_ANTLR_OBJECTS := $(LIBRETS_ANTLR_SRC_FILES:.cpp=.o)
LIBRETS_ANTLR_DEPENDS := $(LIBRETS_ANTLR_SRC_FILES:.cpp=.d)
LIBRETS_ANTLR_CFLAGS = $(CFLAGS) $(CPPFLAGS) $(BOOST_CFLAGS)

ANTLR_FLAGS = -o $(LIBRETS_ANTLR_SRC_DIR)
ANTLR_TREE_FLAGS = $(ANTLR_FLAGS) -glib $(LIBRETS_ANTLR_PARSER)

$(LIBRETS_ANTLR_TRIGGER): $(LIBRETS_ANTLR_GRAMMARS)
	$(ANTLR) $(ANTLR_FLAGS) $(LIBRETS_ANTLR_PARSER)
	$(ANTLR) $(ANTLR_TREE_FLAGS) $(LIBRETS_ANTLR_TREE_PARSER)
	$(ANTLR) $(ANTLR_TREE_FLAGS) $(LIBRETS_ANTLR_GET_OBJECT_TREE_PARSER)
	touch $(LIBRETS_ANTLR_TRIGGER)

$(LIBRETS_ANTLR_HDR_FILES): $(LIBRETS_ANTLR_TRIGGER)
$(LIBRETS_ANTLR_SRC_FILES): $(LIBRETS_ANTLR_TRIGGER)
$(LIBRETS_ANTLR_OBJECTS): $(LIBRETS_ANTLR_TRIGGER)
$(LIBRETS_ANTLR_DEPENDS): $(LIBRETS_ANTLR_TRIGGER)

$(LIBRETS_ANTLR_OBJECTS): \
	$(LIBRETS_ANTLR_OBJ_DIR)/%.o:  $(LIBRETS_ANTLR_SRC_DIR)/%.cpp
	$(CXX) $(LIBRETS_ANTLR_CFLAGS) -I$(LIBRETS_INC_DIR) -c $< -o $@

$(LIBRETS_ANTLR_DEPENDS): \
	$(LIBRETS_ANTLR_OBJ_DIR)/%.d: $(LIBRETS_ANTLR_SRC_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(LIBRETS_ANTLR_CFLAGS) -I$(LIBRETS_INC_DIR) $< \
	| $(FIXDEP) $(LIBRETS_ANTLR_SRC_DIR) $(LIBRETS_ANTLR_OBJ_DIR) > $@

#####

LIBRETS_CFLAGS = $(CFLAGS) $(CPPFLAGS) $(CURL_CFLAGS) $(EXPAT_CFLAGS) \
	$(BOOST_CFLAGS) -I$(LIBRETS_INC_DIR)

ifeq ($(USE_SQL_COMPILER),1)
LIBRETS_SQL_DIR = $(top_srcdir)/project/librets/sql
LIBRETS_SQL_OBJ_DIR = build/librets/sql
LIBRETS_SQL_FILES := $(wildcard ${LIBRETS_SQL_DIR}/*.cpp)
LIBRETS_SQL_OBJECTS := $(patsubst $(LIBRETS_SQL_DIR)/%.cpp, \
	$(LIBRETS_SQL_OBJ_DIR)/%.o, $(LIBRETS_SQL_FILES))

LIBRETS_SQL_DEPENDS := $(patsubst $(LIBRETS_SQL_DIR)/%.cpp, \
	$(LIBRETS_SQL_OBJ_DIR)/%.d, $(LIBRETS_SQL_FILES))
LIBRETS_SQL_DEPENDS += $(LIBRETS_ANTLR_DEPENDS)
LIBRETS_SQL_CFLAGS = $(LIBRETS_CFLAGS) -I$(LIBRETS_ANTLR_SRC_DIR)

$(filter $(LIBRETS_SQL_OBJ_DIR)/%.o, $(LIBRETS_SQL_OBJECTS)): \
	$(LIBRETS_SQL_OBJ_DIR)/%.o: $(LIBRETS_SQL_DIR)/%.cpp
	$(CXX) $(LIBRETS_SQL_CFLAGS) -c $< -o $@

$(filter $(LIBRETS_SQL_OBJ_DIR)/%.d, $(LIBRETS_SQL_DEPENDS)): \
	$(LIBRETS_SQL_OBJ_DIR)/%.d: $(LIBRETS_SQL_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(LIBRETS_SQL_CFLAGS) $< \
	| $(FIXDEP) $(LIBRETS_SQL_DIR) $(LIBRETS_SQL_OBJ_DIR) > $@

endif

#####

LIBRETS_SRC_DIR = $(top_srcdir)/project/librets/src
LIBRETS_INC_DIR = $(top_srcdir)/project/librets/include
LIBRETS_OBJ_DIR = build/librets/objects
LIBRETS_LIB    = build/librets/lib/librets.a
LIBRETS_SRC_FILES := $(wildcard ${LIBRETS_SRC_DIR}/*.cpp)
LIBRETS_OBJECTS := $(patsubst $(LIBRETS_SRC_DIR)/%.cpp, \
	$(LIBRETS_OBJ_DIR)/%.o, $(LIBRETS_SRC_FILES))
LIBRETS_DEPENDS := $(patsubst $(LIBRETS_SRC_DIR)/%.cpp, \
	$(LIBRETS_OBJ_DIR)/%.d, $(LIBRETS_SRC_FILES))


#######

ifeq ($(USE_SQL_COMPILER),1)
LIBRETS_OBJECTS :=  $(LIBRETS_ANTLR_OBJECTS) $(LIBRETS_SQL_OBJECTS) $(LIBRETS_OBJECTS)
LIBRETS_DEPENDS += $(LIBRETS_SQL_DEPENDS) $(LIBRETS_ANTLR_DEPENDS)
LIBRETS_CFLAGS += -I$(LIBRETS_ANTLR_SRC_DIR)
endif

#######

LIBRETS_INC_FILE := ${LIBRETS_INC_DIR}/librets.h
LIBRETS_INC_FILES := $(wildcard ${LIBRETS_INC_DIR}/librets/*.h)

$(filter $(LIBRETS_OBJ_DIR)/%.o, $(LIBRETS_OBJECTS)): \
	$(LIBRETS_OBJ_DIR)/%.o: $(LIBRETS_SRC_DIR)/%.cpp
	$(CXX) $(LIBRETS_CFLAGS) -c $< -o $@

$(filter $(LIBRETS_OBJ_DIR)/%.d, $(LIBRETS_DEPENDS)): \
	$(LIBRETS_OBJ_DIR)/%.d: $(LIBRETS_SRC_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(LIBRETS_CFLAGS) $< \
	| $(FIXDEP) $(LIBRETS_SRC_DIR) $(LIBRETS_OBJ_DIR) > $@

$(LIBRETS_LIB): $(LIBRETS_OBJECTS)
	$(AR) -rs $(LIBRETS_LIB) $(LIBRETS_OBJECTS)

########################################################################
#
# librets test
#

LIBRETS_TEST_SRC_DIR	= $(top_srcdir)/project/librets/test/src
LIBRETS_TEST_INC_DIR	= 
LIBRETS_TEST_OBJ_DIR	= build/librets/test/objects
LIBRETS_TEST_CFLAGS = $(CFLAGS) $(BOOST_CFLAGS) $(CPPUNIT_CFLAGS)

ifeq ($(USE_SQL_COMPILER),1)
LIBRETS_TEST_SQL_SRC_DIR = $(top_srcdir)/project/librets/test/sql
LIBRETS_TEST_SQL_OBJ_DIR = build/librets/test/objects
LIBRETS_TEST_SQL_SRC_FILES := $(wildcard $(LIBRETS_TEST_SQL_SRC_DIR)/*.cpp)
LIBRETS_TEST_SQL_OBJECTS := $(patsubst $(LIBRETS_TEST_SQL_SRC_DIR)/%.cpp, \
	$(LIBRETS_TEST_SQL_OBJ_DIR)/%.o, $(LIBRETS_TEST_SQL_SRC_FILES))
LIBRETS_TEST_SQL_DEPENDS := $(patsubst $(LIBRETS_TEST_SQL_SRC_DIR)/%.cpp, \
	$(LIBRETS_TEST_SQL_OBJ_DIR)/%.d, $(LIBRETS_TEST_SQL_SRC_FILES))

LIBRETS_TEST_SQL_CFLAGS = $(LIBRETS_TEST_CFLAGS) -I$(LIBRETS_TEST_SRC_DIR)

$(LIBRETS_TEST_SQL_OBJ_DIR)/%.o: $(LIBRETS_TEST_SQL_SRC_DIR)/%.cpp
	$(CXX) $(LIBRETS_TEST_SQL_CFLAGS) -I$(LIBRETS_INC_DIR) -c $< -o $@

$(LIBRETS_TEST_SQL_OBJ_DIR)/%.d: $(LIBRETS_TEST_SQL_SRC_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(LIBRETS_TEST_SQL_CFLAGS) -I$(LIBRETS_INC_DIR) $< \
	| $(FIXDEP) $(LIBRETS_TEST_SQL_SRC_DIR) $(LIBRETS_TEST_SQL_OBJ_DIR) > $@
endif

LIBRETS_TEST_SRC_FILES	:= $(wildcard $(LIBRETS_TEST_SRC_DIR)/*.cpp)
LIBRETS_TEST_OBJECTS	:= $(patsubst $(LIBRETS_TEST_SRC_DIR)/%.cpp, \
	$(LIBRETS_TEST_OBJ_DIR)/%.o, $(LIBRETS_TEST_SRC_FILES))
LIBRETS_TEST_DEPENDS	:= $(patsubst $(LIBRETS_TEST_SRC_DIR)/%.cpp, \
	$(LIBRETS_TEST_OBJ_DIR)/%.d, $(LIBRETS_TEST_SRC_FILES))
LIBRETS_TEST_EXE	= build/librets/test/bin/test

ifeq ($(USE_SQL_COMPILER),1)
LIBRETS_TEST_OBJECTS += $(LIBRETS_TEST_SQL_OBJECTS)
LIBRETS_TEST_DEPENDS += $(LIBRETS_TEST_SQL_DEPENDS)
endif

dld-test:
	@echo $(LIBRETS_TEST_SQL_CFLAGS)

$(LIBRETS_TEST_OBJ_DIR)/%.o: $(LIBRETS_TEST_SRC_DIR)/%.cpp
	$(CXX) $(LIBRETS_TEST_CFLAGS) -I$(LIBRETS_INC_DIR) -c $< -o $@

$(LIBRETS_TEST_OBJ_DIR)/%.d: $(LIBRETS_TEST_SRC_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(LIBRETS_TEST_CFLAGS) -I$(LIBRETS_INC_DIR) $< \
	| $(FIXDEP) $(LIBRETS_TEST_SRC_DIR) $(LIBRETS_TEST_OBJ_DIR) > $@

$(LIBRETS_TEST_EXE): $(LIBRETS_TEST_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(LIBRETS_TEST_EXE) $(LIBRETS_TEST_OBJECTS) $(LIBRETS_LIB) \
	$(LIBRETS_LDFLAGS) $(CPPUNIT_LDFLAGS)

########################################################################
#
# examples
#

EXAMPLES_SRC_DIR = $(top_srcdir)/project/examples/src
EXAMPLES_OBJ_DIR = build/examples/objects
EXAMPLES_LDFLAGS = $(LDFLAGS) $(LIBRETS_LDFLAGS) $(BOOST_PROGRAM_OPTIONS)
EXAMPLES_CFLAGS = $(CFLAGS) $(CPPFLAGS) $(BOOST_CFLAGS)

OPTIONS_EXAMPLE_SRC = $(EXAMPLES_SRC_DIR)/Options.cpp
LOGIN_EXAMPLE_SRC_FILES := $(EXAMPLES_SRC_DIR)/login.cpp $(OPTIONS_EXAMPLE_SRC)
LOGIN_EXAMPLE_OBJECTS	 := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(LOGIN_EXAMPLE_SRC_FILES))
LOGIN_EXAMPLE_DEPENDS	 := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(LOGIN_EXAMPLE_SRC_FILES))
LOGIN_EXE = build/examples/bin/login

SEARCH_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/search.cpp \
	$(OPTIONS_EXAMPLE_SRC)
SEARCH_EXAMPLE_OBJECTS	 := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(SEARCH_EXAMPLE_SRC_FILES))
SEARCH_EXAMPLE_DEPENDS	 := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(SEARCH_EXAMPLE_SRC_FILES))
SEARCH_EXE = build/examples/bin/search

DEMO_SEARCH_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/demo-search.cpp \
	$(OPTIONS_EXAMPLE_SRC)
DEMO_SEARCH_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(DEMO_SEARCH_EXAMPLE_SRC_FILES))
DEMO_SEARCH_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(DEMO_SEARCH_EXAMPLE_SRC_FILES))
DEMO_SEARCH_EXE = build/examples/bin/demo-search

METADATA_EXAMPLE_SRC_FILES := $(EXAMPLES_SRC_DIR)/metadata.cpp \
	$(OPTIONS_EXAMPLE_SRC)
METADATA_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(METADATA_EXAMPLE_SRC_FILES))
METADATA_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(METADATA_EXAMPLE_SRC_FILES))
METADATA_EXE = build/examples/bin/metadata

XML_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/xml.cpp
XML_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(XML_EXAMPLE_SRC_FILES))
XML_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(XML_EXAMPLE_SRC_FILES))
XML_EXE = build/examples/bin/xml

HTTP_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/http.cpp
HTTP_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(HTTP_EXAMPLE_SRC_FILES))
HTTP_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(HTTP_EXAMPLE_SRC_FILES))
HTTP_EXE = build/examples/bin/http

RAW_RETS_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/raw-rets.cpp
RAW_RETS_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(RAW_RETS_EXAMPLE_SRC_FILES))
RAW_RETS_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(RAW_RETS_EXAMPLE_SRC_FILES))
RAW_RETS_EXE = build/examples/bin/raw-rets

SQL2DMQL_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/sql2dmql.cpp
SQL2DMQL_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(SQL2DMQL_EXAMPLE_SRC_FILES))
SQL2DMQL_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(SQL2DMQL_EXAMPLE_SRC_FILES))
SQL2DMQL_EXE = build/examples/bin/sql2dmql

GET_OBJECT_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/get-object.cpp \
	$(OPTIONS_EXAMPLE_SRC)
GET_OBJECT_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(GET_OBJECT_EXAMPLE_SRC_FILES))
GET_OBJECT_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(GET_OBJECT_EXAMPLE_SRC_FILES))
GET_OBJECT_EXE = build/examples/bin/get-object

EXAMPLES_DEPENDS = $(LOGIN_EXAMPLE_DEPENDS) $(SEARCH_EXAMPLE_DEPENDS) \
	$(METADATA_EXAMPLE_DEPENDS) $(XML_EXAMPLE_DEPENDS) \
	$(HTML_EXAMPLE_DEPENDS) $(SQL2DMQL_EXAMPLE_DEPENDS) \
	$(HTTP_EXAMPLE_DEPENDS) $(GET_OBJECT_EXAMPLE_DEPENDS) \
	$(DEMO_SEARCH_EXAMPLE_DEPENDS)

EXAMPLES_EXE = $(LOGIN_EXE) $(SEARCH_EXE) $(METADATA_EXE) $(XML_EXE) \
	$(HTTP_EXE) $(RAW_RETS_EXE) $(GET_OBJECT_EXE) \
	$(DEMO_SEARCH_EXE)

ifeq ($(USE_SQL_COMPILER),1)
EXAMPLES_EXE += $(SQL2DMQL_EXE)
endif

$(EXAMPLES_OBJ_DIR)/%.o: $(EXAMPLES_SRC_DIR)/%.cpp
	$(CXX) $(EXAMPLES_CFLAGS) -I$(LIBRETS_INC_DIR) -c $< -o $@

$(EXAMPLES_OBJ_DIR)/%.d: $(EXAMPLES_SRC_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(EXAMPLES_CFLAGS) -I$(LIBRETS_INC_DIR) $< \
	| $(FIXDEP) $(EXAMPLES_SRC_DIR) $(EXAMPLES_OBJ_DIR) > $@


$(LOGIN_EXE): $(LOGIN_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(LOGIN_EXE) $(LOGIN_EXAMPLE_OBJECTS) $(LIBRETS_LIB) \
	$(EXAMPLES_LDFLAGS)

$(SEARCH_EXE): $(SEARCH_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(SEARCH_EXE) $(SEARCH_EXAMPLE_OBJECTS) $(LIBRETS_LIB) \
	$(EXAMPLES_LDFLAGS)

$(DEMO_SEARCH_EXE): $(DEMO_SEARCH_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(DEMO_SEARCH_EXE) $(DEMO_SEARCH_EXAMPLE_OBJECTS) \
	$(LIBRETS_LIB) $(EXAMPLES_LDFLAGS)

$(METADATA_EXE): $(METADATA_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(METADATA_EXE) $(METADATA_EXAMPLE_OBJECTS) $(LIBRETS_LIB) \
	$(EXAMPLES_LDFLAGS)

$(XML_EXE): $(XML_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(XML_EXE) $(XML_EXAMPLE_OBJECTS) $(LIBRETS_LIB) \
	$(EXAMPLES_LDFLAGS)

$(HTTP_EXE): $(HTTP_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(HTTP_EXE) $(HTTP_EXAMPLE_OBJECTS) $(LIBRETS_LIB) \
	$(EXAMPLES_LDFLAGS)

$(RAW_RETS_EXE): $(RAW_RETS_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(RAW_RETS_EXE) $(RAW_RETS_EXAMPLE_OBJECTS) $(LIBRETS_LIB) \
	$(EXAMPLES_LDFLAGS)

$(SQL2DMQL_EXE): $(SQL2DMQL_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(SQL2DMQL_EXE) $(SQL2DMQL_EXAMPLE_OBJECTS) $(LIBRETS_LIB) \
	$(EXAMPLES_LDFLAGS)

$(GET_OBJECT_EXE): $(GET_OBJECT_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(GET_OBJECT_EXE) $(GET_OBJECT_EXAMPLE_OBJECTS) \
	$(LIBRETS_LIB) $(EXAMPLES_LDFLAGS)


########################################################################
#
# swig
#
ifeq (${USE_SWIG_BINDINGS}, 1)

SWIG_DEFAULT		=
# SWIG_DEPENDS		= $(shell find ${SWIG_DIR} -name "*.i")
SWIG_DIR		= ${top_srcdir}/project/swig
SWIG_FILES		= ${SWIG_DIR}/librets.i ${SWIG_DIR}/auto_ptr_release.i
SWIG_LIBRETS_CONFIG	= ${top_srcdir}/librets-config-inplace
SWIG_LIBRETS_LIBS	= `${SWIG_LIBRETS_CONFIG} --libs`
SWIG_OBJ_DIR		= build/swig
SWIG_OSNAME		= $(shell perl -e 'use Config; print $$Config{osname};')

ifeq (${SWIG_OSNAME}, darwin)
SWIG_LINK		= ${CXX} -bundle -undefined suppress -flat_namespace 
else
SWIG_LINK		= ${CXX} -shared
endif


###
# csharp of swig
#
ifeq (${HAVE_MCS},1)

CSHARP_ALL		= ${CSHARP_MANAGED_DLL}					\
				${CSHARP_UNMANAGED_DLL}				\
				${CSHARP_DEMO_EXE}

CSHARP_BUILD		= ${CSHARP_WRAP}  ${CSHARP_ALL}
CSHARP_CXX_FLAGS	= `${SWIG_LIBRETS_CONFIG} --cflags`
CSHARP_DEMO_EXE		= ${CSHARP_GETOBJECT_EXE}				\
				${CSHARP_METADATA_EXE}				\
				${CSHARP_LOGIN_EXE}				\
				${CSHARP_SEARCH_EXE}				
CSHARP_DEMO_SRC		= ${CSHARP_GETOBJECT_SRC}				\
				${CSHARP_METADATA_SRC}				\
				${CSHARP_LOGIN_SRC}				\
				${CSHARP_SEARCH_SRC}				
CSHARP_DIR		= ${SWIG_DIR}/csharp
CSHARP_GENERATED_SRC	= ${wildcard ${CSHARP_OBJ_DIR}/*.cs}
CSHARP_GETOBJECT_EXE	= ${CSHARP_OBJ_DIR}/GetObject.exe
CSHARP_GETOBJECT_SRC	= ${CSHARP_DIR}/GetObject.cs
CSHARP_INSTALL		= csharp_install
CSHARP_LOGIN_EXE	= ${CSHARP_OBJ_DIR}/Login.exe
CSHARP_LOGIN_SRC	= ${CSHARP_DIR}/Login.cs
CSHARP_MANAGED_DLL	= ${CSHARP_OBJ_DIR}/librets-dotnet.dll
CSHARP_MANAGED_SRC	= ${CSHARP_GENERATED_SRC}		 		\
				${CSHARP_DIR}/CppInputStream.cs 		\
        			${CSHARP_DIR}/ObjectDescriptorEnumerator.cs 	\
				${CSHARP_DIR}/TextWriterLogger.cs

CSHARP_METADATA_EXE	= ${CSHARP_OBJ_DIR}/Metadata.exe
CSHARP_METADATA_SRC	= ${CSHARP_DIR}/Metadata.cs
CSHARP_OBJ_DIR		= ${SWIG_OBJ_DIR}/csharp
CSHARP_SEARCH_EXE	= ${CSHARP_OBJ_DIR}/Search.exe
CSHARP_SEARCH_SRC	= ${CSHARP_DIR}/Search.cs
CSHARP_SQL2DMQL_EXE	= ${CSHARP_OBJ_DIR}/Sql2DMQL.exe
CSHARP_SQL2DMQL_SRC	= ${CSHARP_DIR}/Sql2DMQL.cs ${CSHARP_DIR}/SimpleSqlMetadata.cs
CSHARP_UNMANAGED_DLL	= ${CSHARP_OBJ_DIR}/librets.so
CSHARP_UNMANAGED_OBJ	= ${CSHARP_OBJ_DIR}/librets_wrap.o 			\
				${CSHARP_OBJ_DIR}/librets_sharp.o
CSHARP_WRAP		= ${CSHARP_OBJ_DIR}/librets_wrap.cpp

${CSHARP_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -csharp -namespace librets -o ${CSHARP_WRAP} \
	-outdir ${CSHARP_OBJ_DIR} -I${SWIG_DIR}/lib/csharp ${SWIG_DIR}/librets.i
	make ${CSHARP_MANAGED_DLL}

${CSHARP_UNMANAGED_DLL}: ${CSHARP_UNMANAGED_OBJ}
	${SWIG_LINK} -o ${CSHARP_UNMANAGED_DLL} ${CSHARP_UNMANAGED_OBJ} ${SWIG_LIBRETS_LIBS}

${CSHARP_MANAGED_DLL}:	${CSHARP_UNMANAGED_DLL} ${CSHARP_MANAGED_SRC}
	${MCS} -target:library -out:${CSHARP_MANAGED_DLL} ${CSHARP_MANAGED_SRC}

${CSHARP_OBJ_DIR}/%.o: ${CSHARP_OBJ_DIR}/%.cpp 
	${CXX} ${CSHARP_CXX_FLAGS} -I${LIBRETS_INC_DIR} -I${CSHARP_OBJ_DIR} -I${CSHARP_DIR}  -c $< -o $@

${CSHARP_OBJ_DIR}/%.o: ${CSHARP_DIR}/%.cpp
	${CXX} ${CSHARP_CXX_FLAGS} -I${LIBRETS_INC_DIR} -I${CSHARP_OBJ_DIR} -I${CSHARP_DIR}  -c $< -o $@

${CSHARP_GETOBJECT_EXE}:	${CSHARP_GETOBJECT_SRC}
	${MCS} -r:${CSHARP_MANAGED_DLL} -out:${CSHARP_GETOBJECT_EXE} ${CSHARP_GETOBJECT_SRC}

${CSHARP_METADATA_EXE}:		${CSHARP_METADATA_SRC}
	${MCS} -r:${CSHARP_MANAGED_DLL} -out:${CSHARP_METADATA_EXE} ${CSHARP_METADATA_SRC}

${CSHARP_LOGIN_EXE}:		${CSHARP_LOGIN_SRC}
	${MCS} -r:${CSHARP_MANAGED_DLL} -out:${CSHARP_LOGIN_EXE}  ${CSHARP_LOGIN_SRC}

${CSHARP_SEARCH_EXE}:		${CSHARP_SEARCH_SRC}
	${MCS} -r:${CSHARP_MANAGED_DLL} -out:${CSHARP_SEARCH_EXE}  ${CSHARP_SEARCH_SRC}

${CSHARP_SQL2DMQL_EXE}:		${CSHARP_SQL2DMQL_SRC}
	${MCS} -r:${CSHARP_MANAGED_DLL} -main:Sql2DMQL -out:${CSHARP_SQL2DMQL_EXE} ${CSHARP_SQL2DMQL_SRC}

${CSHARP_INSTALL}: ${CSHARP_MANAGED_DLL}
	@echo The csharp assemblies can be found in: ${CSHARP_OBJ_DIR}. They will need to be manually
	@echo insalled in your environment.
endif

###
# java of swig
#
ifeq (${HAVE_JAVA},1)

JAVA_BUILD		= ${JAVA_DLL} ${JAVA_OBJ_DIR}/${JAVA_JAR} ${JAVA_EXAMPLES_CLASSES}

JAVA_CLASSES		= ${patsubst ${JAVA_OBJ_DIR}/%.java,${JAVA_OBJ_DIR}/librets/%.class,${JAVA_SOURCES}}
JAVA_CLASSES_UNQUAL	= ${patsubst ${JAVA_OBJ_DIR}/%.java,%.class,${JAVA_SOURCES}}
JAVA_EXAMPLES		= ${wildcard ${JAVA_SRC_DIR}/*.java}
JAVA_EXAMPLES_CLASSES	= ${patsubst ${JAVA_SRC_DIR}/%.java,${JAVA_OBJ_DIR}/%.class,${JAVA_EXAMPLES}}
JAVA_JAR		= librets.jar
JAVA_OBJ_DIR		= ${SWIG_OBJ_DIR}/java
JAVA_SOURCES		= ${wildcard ${JAVA_OBJ_DIR}/*.java}
JAVA_SRC_DIR		= ${SWIG_DIR}/java
JAVA_WRAP 		= ${JAVA_OBJ_DIR}/librets_wrap.cpp

JAVA_INCLUDES		= ${BOOST_CFLAGS}

ifeq (${SWIG_OSNAME}, darwin)
JAVA_CLASSPATH		= `javaconfig DefaultClasspath`:${JAVA_OBJ_DIR}/${JAVA_JAR}
JAVA_INCLUDES		= -I`javaconfig Headers`
JAVA_DLL		= ${JAVA_OBJ_DIR}/liblibrets.jnilib
#JAVA_DYNAMICLINK	= ${CXX} -dynamiclib -framework JavaVM
JAVA_DYNAMICLINK	= ${SWIG_LINK}
else
JAVA_CLASSPATH		= `echo You must provide JAVA_CLASSPATH`
JAVA_INCLUDES		= `echo You must provide JAVA_INCLUDES`
JAVA_DLL		= ${JAVA_OBJ_DIR}/liblibrets.so
JAVA_DYNAMICLINK	= ${SWIG_LINK}
endif

${JAVA_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -java -package librets -o ${JAVA_WRAP} \
	-outdir ${JAVA_OBJ_DIR} ${SWIG_DIR}/librets.i
	${MAKE} ${JAVA_OBJ_DIR}/${JAVA_JAR}

${JAVA_DLL}: ${JAVA_WRAP} ${JAVA_OBJ_DIR}/librets_wrap.o
	${JAVA_DYNAMICLINK} -o ${JAVA_DLL} ${JAVA_OBJ_DIR}/librets_wrap.o ${SWIG_LIBRETS_LIBS}

${JAVA_OBJ_DIR}/librets_wrap.o: ${JAVA_OBJ_DIR}/librets_wrap.cpp
	${CXX}  -I${LIBRETS_INC_DIR}  ${BOOST_CFLAGS} ${JAVA_INCLUDES} -c $< -o $@
	
${JAVA_CLASSES}: ${JAVA_WRAP} ${JAVA_SOURCES}
	${JAVAC} -d ${JAVA_OBJ_DIR} ${JAVA_SOURCES}

${JAVA_OBJ_DIR}/${JAVA_JAR}: ${JAVA_CLASSES}
	cd ${JAVA_OBJ_DIR}; ${JAR} -cvf ${JAVA_JAR} librets || \
					${RM} ${JAVA_OBJ_DIR}/${JAVA_JAR}

${JAVA_EXAMPLES_CLASSES}: ${JAVA_EXAMPLES} ${JAVA_OBJ_DIR}/${JAVA_JAR}
	${JAVAC} -classpath ${JAVA_CLASSPATH} -d ${JAVA_OBJ_DIR} ${JAVA_EXAMPLES}
endif
###

###
# php of swig
#
ifeq (${HAVE_PHP},1)

PHP_BUILD		= ${PHP_DLL} 

PHP_DLL			= ${PHP_OBJ_DIR}/librets.so
PHP_INCLUDES		= `php-config --includes`
PHP_LDFLAGS		= `php-config --ldflags`
PHP_LIBS		= `php-config --libs`
PHP_OBJ_DIR		= ${SWIG_OBJ_DIR}/php5
PHP_SRC_DIR		= ${SWIG_DIR}/php5
PHP_WRAP 		= ${PHP_OBJ_DIR}/librets_wrap.cpp

${PHP_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -php5 -o ${PHP_WRAP} \
	-outdir ${PHP_OBJ_DIR} ${SWIG_DIR}/librets.i

${PHP_DLL}: ${PHP_WRAP} ${PHP_OBJ_DIR}/librets_wrap.o
	${SWIG_LINK} -o ${PHP_DLL} ${PHP_LDFLAGS} ${PHP_LIBS} ${PHP_OBJ_DIR}/librets_wrap.o ${SWIG_LIBRETS_LIBS}

${PHP_OBJ_DIR}/librets_wrap.o: ${PHP_OBJ_DIR}/librets_wrap.cpp
	${CXX} -g  -I${LIBRETS_INC_DIR} -I${PHP_SRC_DIR}  ${BOOST_CFLAGS} ${PHP_INCLUDES} -c $< -o $@
	
endif
###
# perl of swig - Perl isn't completely implemented and currently won't build, so this section is commented out.
#
ifeq (0,1)

PERL_BUILD		= ${PERL_DLL}

PERL_DLL		= ${PERL_OBJ_DIR}/librets_native.bundle
PERL_EXTCONF_RB		= ${PERL_SRC_DIR}/extconf.rb
PERL_MAKEFILE		= ${PERL_OBJ_DIR}/Makefile
PERL_MAKEFILE_PL	= ${PERL_OBJ_DIR}/Makefile.PL
PERL_OBJ_DIR		= ${SWIG_OBJ_DIR}/perl
PERL_SRC_DIR		= ${SWIG_DIR}/perl
PERL_WRAP 		= ${PERL_OBJ_DIR}/librets_wrap.cpp

${PERL_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -perl -o ${PERL_WRAP} \
	-outdir ${PERL_OBJ_DIR} ${SWIG_DIR}/librets.i

${PERL_MAKEFILE}: ${PERL_WRAP}
	cd ${PERL_OBJ_DIR}; ${PERL}  ../../../${PERL_MAKEFILE_PL}

${PERL_DLL}: ${PERL_MAKEFILE}
	${MAKE} -C ${PERL_OBJ_DIR}
	

endif

###
# python of swig
#
ifeq (${HAVE_PYTHON},1)

PYTHON_BUILD		= ${PYTHON_DLL}

PYTHON_DLL		= ${PYTHON_OBJ_DIR}/_librets.so
PYTHON_INSTALL		= python_install
PYTHON_OBJ_DIR		= ${SWIG_OBJ_DIR}/python
PYTHON_SRC_DIR		= ${SWIG_DIR}/python
PYTHON_WRAP 		= ${PYTHON_OBJ_DIR}/librets_wrap.cpp

${PYTHON_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -python -o ${PYTHON_WRAP} \
	-outdir ${PYTHON_OBJ_DIR} ${SWIG_DIR}/librets.i
	@cp ${PYTHON_SRC_DIR}/* ${PYTHON_OBJ_DIR}

${PYTHON_DLL}: ${PYTHON_WRAP}
	cd ${PYTHON_OBJ_DIR} ; ${PYTHON} setup.py build --build-lib=.
	
${PYTHON_INSTALL}: ${PYTHON_DLL}
	cd ${PYTHON_OBJ_DIR}; ${PYTHON} setup.py install

endif

###
# ruby of swig
#
ifeq (${HAVE_RUBY},1)

RUBY_BUILD		= ${RUBY_DLL}

RUBY_DLL		= ${RUBY_OBJ_DIR}/librets_native.bundle
RUBY_EXTCONF_RB		= extconf.rb
RUBY_INSTALL		= ruby_install
RUBY_MAKEFILE		= ${RUBY_OBJ_DIR}/Makefile
RUBY_OBJ_DIR		= ${SWIG_OBJ_DIR}/ruby
RUBY_SRC_DIR		= ${SWIG_DIR}/ruby
RUBY_WRAP 		= ${RUBY_OBJ_DIR}/librets_wrap.cpp

${RUBY_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -ruby -o ${RUBY_WRAP} -module librets_native \
	-outdir ${RUBY_OBJ_DIR} -I${SWIG_DIR}/lib/ruby ${SWIG_DIR}/librets.i

${RUBY_MAKEFILE}: ${RUBY_WRAP} ${RUBY_SRC_DIR}/extconf.rb
	cp ${RUBY_SRC_DIR}/* ${RUBY_OBJ_DIR}
	${RUBY} -C ${RUBY_OBJ_DIR} ${RUBY_EXTCONF_RB} 		\
	--with-librets-config=../../../${SWIG_LIBRETS_CONFIG}	\
	--with-swig-dir=../../../${SWIG_DIR}

${RUBY_DLL}: ${RUBY_MAKEFILE}
	${MAKE} -C ${RUBY_OBJ_DIR}
	
${RUBY_INSTALL}: ${RUBY_DLL}
	${MAKE} -C ${RUBY_OBJ_DIR} install

endif


endif

########################################################################
#
# misc
#

DISTCLEAN_FILES = \
	config.status config.log config.cache \
	project/librets/src/config.h \
	Makefile project/build/Doxyfile \
	librets-config librets-config-inplace

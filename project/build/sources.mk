########################################################################
#
# librets
#

LIBRETS_SRC_DIR = project/librets/src
LIBRETS_INC_DIR = project/librets/include
LIBRETS_OBJ_DIR = build/librets/objects
LIBRETS_SRC_FILES := $(wildcard ${LIBRETS_SRC_DIR}/*.cpp)
LIBRETS_OBJECTS	:= $(patsubst $(LIBRETS_SRC_DIR)/%.cpp, \
	$(LIBRETS_OBJ_DIR)/%.o, $(LIBRETS_SRC_FILES))
LIBRETS_DEPENDS	:= $(patsubst $(LIBRETS_SRC_DIR)/%.cpp, \
	$(LIBRETS_OBJ_DIR)/%.d, $(LIBRETS_SRC_FILES))
LIBRETS_LIB	= build/librets/lib/librets.a

$(LIBRETS_OBJ_DIR)/%.o: $(LIBRETS_SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(LIBRETS_INC_DIR) -c $< -o $@

$(LIBRETS_OBJ_DIR)/%.d: $(LIBRETS_SRC_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(CFLAGS) -I$(LIBRETS_INC_DIR) $< \
	| $(FIXDEP) $(LIBRETS_SRC_DIR) $(LIBRETS_OBJ_DIR) > $@

$(LIBRETS_LIB): $(LIBRETS_OBJECTS)
	$(AR) -rs $(LIBRETS_LIB) $(LIBRETS_OBJECTS)

########################################################################
#
# examples
#

EXAMPLES_SRC_DIR = project/examples/src
EXAMPLES_OBJ_DIR = build/examples/objects
SEARCH_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/search.cpp
SEARCH_EXAMPLE_OBJECTS	 := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(SEARCH_EXAMPLE_SRC_FILES))
SEARCH_EXAMPLE_DEPENDS	 := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(SEARCH_EXAMPLE_SRC_FILES))
SEARCH_EXE = build/examples/bin/search

METADATA_EXAMPLE_SRC_FILES := ${EXAMPLES_SRC_DIR}/metadata.cpp
METADATA_EXAMPLE_OBJECTS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.o, $(METADATA_EXAMPLE_SRC_FILES))
METADATA_EXAMPLE_DEPENDS := $(patsubst $(EXAMPLES_SRC_DIR)/%.cpp, \
	$(EXAMPLES_OBJ_DIR)/%.d, $(METADATA_EXAMPLE_SRC_FILES))
METADATA_EXE = build/examples/bin/metadata

EXAMPLES_DEPENDS = $(SEARCH_EXAMPLE_DEPENDS) $(METADATA_EXAMPLE_DEPENDS)
EXAMPLES_EXE = $(SEARCH_EXE) $(METADATA_EXE)

$(EXAMPLES_OBJ_DIR)/%.o: $(EXAMPLES_SRC_DIR)/%.cpp
	$(CXX) $(CFLAGS) -I$(LIBRETS_INC_DIR) -c $< -o $@

$(EXAMPLES_OBJ_DIR)/%.d: $(EXAMPLES_SRC_DIR)/%.cpp
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(CFLAGS) -I$(LIBRETS_INC_DIR) $< \
	| $(FIXDEP) $(EXAMPLES_SRC_DIR) $(EXAMPLES_OBJ_DIR) > $@


$(SEARCH_EXE): $(SEARCH_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(SEARCH_EXE) $(SEARCH_EXAMPLE_OBJECTS) $(LIBRETS_LIB)

$(METADATA_EXE): $(METADATA_EXAMPLE_OBJECTS) $(LIBRETS_LIB)
	$(CXX) -o $(METADATA_EXE) $(METADATA_EXAMPLE_OBJECTS) $(LIBRETS_LIB)


########################################################################
#
# misc
#

DISTCLEAN_FILES = \
	config.status config.log config.cache \
	project/librets/src/config.h \
	project/build/Doxyfile

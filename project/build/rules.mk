VERSION		= 1.0.0b1

DIST_SRC	= librets-$(VERSION)
SRC_TGZ		= librets-$(VERSION).tar.gz
SRC_ZIP		= librets-$(VERSION).zip

CFLAGS		+= $(TARGET_CFLAGS) -DLIBRETS_VERSION='"$(VERSION)"'

LIBRETS_LDFLAGS = $(BOOST_LDFLAGS) $(CURL_LDFLAGS) $(EXPAT_LDFLAGS) \
	$(ANTLR_LDFLAGS)

FIXDEP		= perl $(srcdir)/project/build/fixdep.pl
ALL_DEPENDS 	= $(LIBRETS_DEPENDS) $(LIBRETS_TEST_DEPENDS) \
	$(EXAMPLES_DEPENDS)
ALL_OBJ_DIRS	= \
	build/librets/antlr \
	build/librets/objects \
	build/librets/lib \
	build/librets/test/objects \
	build/librets/test/bin \
	build/examples/objects \
	build/examples/bin \
	build/doc/api

ALL		= $(LIBRETS_LIB) $(EXAMPLES_EXE)

_all: _build

_debug:
	@echo $(USE_DEPENDS)

prepare:
	@mkdir -p $(ALL_OBJ_DIRS) $(EXE_DIR) $(DOC_DIR)

_build: prepare $(ALL)

_doc: prepare $(DOC_FILES_TO_GEN)

_doc-api: prepare
	$(RM) -r build/doc/api
	doxygen project/build/Doxyfile

# (Ab)use rsync as a recursive copy with exclude.  Yeah, rsync is
# "non-standard", but screw it.  It's unfortunately the easiest way to
# do a recursive copy with exclude, especially with the cool
# --cvs-exclude option.  Must manually copy "./configure" over since
# it is ignored in .cvsignore.
_dist: _build _doc-api
	$(RM) -r dist/$(DIST_SRC)
	mkdir -p dist/$(DIST_SRC)
	mkdir -p dist/$(DIST_SRC)/doc
	rsync -a --exclude-from project/build/dist-exclude . \
	dist/$(DIST_SRC)
	rsync -a --cvs-exclude build/doc/api dist/$(DIST_SRC)/doc
	cd dist; zip -r -q $(SRC_ZIP) $(DIST_SRC)
	cd dist; tar --gzip -cf $(SRC_TGZ) $(DIST_SRC)
	$(RM) -r dist/$(DIST_SRC)

_clean:
	$(RM) -r build dist

_distclean: _clean
	$(RM) $(DISTCLEAN_FILES)
	$(RM) -r autom4te.cache
	$(RM) makefile

_veryclean: _distclean

_test: prepare $(LIBRETS_TEST_EXE)
	./$(LIBRETS_TEST_EXE)

_maintainer-clean: _veryclean
	$(RM) configure

# Cancel built-in rules
%.o: %.cpp
%.o: %.c
%.o: %.d

ifeq ($(USE_DEPENDS),1)
-include $(ALL_DEPENDS)
endif

.PHONY: all debug build doc doc-api \
	insatll install-bin install-data install-config no-cppunit \
	_all _debug _build _doc _doc-api _clean _distclean _veryclean \
	_maintainer-clean test _test

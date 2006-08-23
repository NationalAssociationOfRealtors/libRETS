DIST_SRC	= librets-$(VERSION)
SRC_TGZ		= librets-$(VERSION).tar.gz
SRC_ZIP		= librets-$(VERSION).zip

CFLAGS		+= $(TARGET_CFLAGS) -DLIBRETS_VERSION='"$(VERSION)"'

LIBRETS_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM) $(CURL_LDFLAGS) \
	$(EXPAT_LDFLAGS) $(ANTLR_LDFLAGS)

FIXDEP		= perl $(top_srcdir)/project/build/fixdep.pl
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

ALL = $(LIBRETS_LIB)
ifeq ($(USE_EXAMPLES),1)
ALL += $(EXAMPLES_EXE)
endif

cppunit:
ifeq ($(HAVE_CPPUNIT),0)
	@echo "cppunit is not installed"
	@false
endif

doxygen:
ifeq ($(HAVE_DOXYGEN),0)
	@echo "doxgyen is not installed"
	@false
endif

_all: _build

_debug:
	@echo $(USE_DEPENDS)

prepare:
	@mkdir -p $(ALL_OBJ_DIRS) $(EXE_DIR) $(DOC_DIR)

_build: prepare $(ALL)

_doc: prepare $(DOC_FILES_TO_GEN)

_doc-api: doxygen prepare
	$(RM) -r build/doc/api
	$(DOXYGEN) project/build/Doxyfile

# (Ab)use rsync as a recursive copy with exclude.  Yeah, rsync is
# "non-standard", but screw it.  It's unfortunately the easiest way to
# do a recursive copy with exclude.
_dist: _doc-api
	$(RM) -r dist/$(DIST_SRC)
	mkdir -p dist/$(DIST_SRC)
	mkdir -p dist/$(DIST_SRC)/doc
	rsync -a --exclude-from project/build/dist-exclude . \
	  dist/$(DIST_SRC)
	rsync -a --cvs-exclude build/doc/api dist/$(DIST_SRC)/doc
	cd dist; zip -r -q $(SRC_ZIP) $(DIST_SRC)
	cd dist; tar --gzip -cf $(SRC_TGZ) $(DIST_SRC)
	$(RM) -r dist/$(DIST_SRC)

_install: _build
	@$(MKINSTALLDIRS) "$(DESTDIR)$(libdir)"
	$(INSTALL_DATA) $(LIBRETS_LIB) "$(DESTDIR)$(libdir)"
	@$(MKINSTALLDIRS) "$(DESTDIR)$(includedir)/librets"

	$(INSTALL_DATA) $(LIBRETS_INC_FILE) "$(DESTDIR)$(includedir)"
	@list='$(LIBRETS_INC_FILES)'; for p in $$list; do \
	  $(INSTALL_DATA) "$$p" "$(DESTDIR)$(includedir)/librets"; \
	done

	@$(MKINSTALLDIRS) "$(DESTDIR)$(bindir)"
	$(INSTALL_PROGRAM) $(top_srcdir)/librets-config "$(DESTDIR)$(bindir)"

_clean:
	$(RM) -r build dist
	$(MAKE) -C project/swig clean

_distclean: _clean
	$(RM) $(DISTCLEAN_FILES)
	$(RM) -r autom4te.cache
	$(RM) makefile

_veryclean: _distclean

_test: cppunit prepare $(LIBRETS_TEST_EXE)
	./$(LIBRETS_TEST_EXE) $(top_srcdir)

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

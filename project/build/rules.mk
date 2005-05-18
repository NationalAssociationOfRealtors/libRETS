VERSION		= 0.0.1

CFLAGS		+= $(TARGET_CFLAGS) -DLIBRETS_VERSION='"$(VERSION)"'

FIXDEP		= perl $(srcdir)/project/build/fixdep.pl
ALL_DEPENDS 	= $(LIBRETS_DEPENDS) $(EXAMPLES_DEPENDS)
ALL_OBJ_DIRS	= build/librets/objects \
	build/librets/lib \
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
	doxygen project/build/Doxyfile

# (Ab)use rsync as a recursive copy with exclude.  Yeah, rsync is
# "non-standard", but screw it.  It's unfortunately the easiest way to
# do a recursive copy with exclude, especially with the cool
# --cvs-exclude option.  Must manually copy "./configure" over since
# it is ignored in .cvsignore.
_dist:
	$(RM) -r build/glaunch build/$(UNIX_SRC_DIR)
	$(RM) build/$(SRC_ZIP) build/$(SRC_TGZ) build/$(SRC_TBZ2)
	mkdir -p build/glaunch
	rsync -a --cvs-exclude . build/glaunch
	cp configure build/glaunch
	cd build ; zip -qr $(SRC_ZIP) glaunch
	mv build/glaunch build/$(UNIX_SRC_DIR)
	cd build ; tar --gzip -cf $(SRC_TGZ) $(UNIX_SRC_DIR)
	cd build ; tar --bzip -cf $(SRC_TBZ2) $(UNIX_SRC_DIR)
	$(RM) -r build/$(UNIX_SRC_DIR)

_clean:
	$(RM) -r build

_distclean: _clean
	$(RM) $(DISTCLEAN_FILES)
	$(RM) -r autom4te.cache
	$(RM) makefile

_veryclean: _distclean

_maintainer-clean: _veryclean
	$(RM) configure

build/objects/librets/%.d: project/librets/src/%.cc
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(CFLAGS) $< | $(FIXDEP) > $@

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.c
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CC) -MM $(CFLAGS) $< | $(FIXDEP) > $@

$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cc
	@echo Generating dependencies for $<
	@mkdir -p $(dir $@)
	@$(CXX) -MM $(CFLAGS) $< | $(FIXDEP) > $@

ifeq ($(USE_DEPENDS),1)
-include $(ALL_DEPENDS)
endif

.PHONY: all debug build doc doc-api \
	insatll install-bin install-data install-config no-cppunit \
	_all _debug _build _doc _doc-api _clean _distclean _veryclean \
	_maintainer-clean

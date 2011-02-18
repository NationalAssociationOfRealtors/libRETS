DIST_BIN	= librets-$(VERSION)${ARCH}
DIST_SRC	= librets-$(VERSION)
SRC_TGZ		= librets-$(VERSION).tar.gz
SRC_ZIP		= librets-$(VERSION).zip

CFLAGS		+= $(TARGET_CFLAGS) -DLIBRETS_VERSION='"$(VERSION)"'

LIBRETS_LDFLAGS = $(BOOST_LIBS) $(BOOST_FILESYSTEM) $(BOOST_SYSTEM) \
	$(CURL_LDFLAGS) $(EXPAT_LDFLAGS) $(ANTLR_LDFLAGS)

FIXDEP		= perl $(top_srcdir)/project/build/fixdep.pl
ALL_DEPENDS 	= $(LIBRETS_DEPENDS) $(LIBRETS_TEST_DEPENDS) \
	$(EXAMPLES_DEPENDS) ${SWIG_DEPENDS}
ALL_OBJ_DIRS	= \
	${BUILD}/librets/antlr \
	${BUILD}/librets/objects \
	${BUILD}/librets/sql \
	${BUILD}/librets/lib \
	${BUILD}/librets/test/objects \
	${BUILD}/librets/test/sql \
	${BUILD}/librets/test/bin \
	${BUILD}/librets/test-network/bin \
	${BUILD}/librets/test-network/objects \
	${BUILD}/examples/objects \
	${BUILD}/examples/bin \
	${BUILD}/doc/api \
	${BUILD}/swig/csharp \
	${BUILD}/swig/java \
	${BUILD}/swig/java/librets \
	${BUILD}/swig/perl \
	${BUILD}/swig/php5 \
	${BUILD}/swig/python \
	${BUILD}/swig/ruby

ALL = $(LIBRETS_LIB)
ifeq ($(USE_EXAMPLES),1)
ALL += $(EXAMPLES_EXE)
endif

ifeq (${USE_SWIG_BINDINGS},1)
ALL += ${SWIG_DEFAULT}
SWIG_INSTALL = ${SWIG_DEFAULT_INSTALL}

ifeq (${HAVE_DOTNET},1)
ALL += ${DOTNET_BUILD}
SWIG_INSTALL += ${DOTNET_INSTALL}
endif

ifeq (${HAVE_JAVA},1)
ALL += ${JAVA_BUILD}
SWIG_INSTALL += ${JAVA_INSTALL}
endif

ifeq (${HAVE_PERL},1)
ALL += ${PERL_BUILD}
SWIG_INSTALL += ${PERL_INSTALL}
endif

ifeq (${HAVE_PHP},1)
ALL += ${PHP_BUILD}
SWIG_INSTALL += ${PHP_INSTALL}
endif

ifeq (${HAVE_PYTHON},1)
ALL += ${PYTHON_BUILD}
SWIG_INSTALL += ${PYTHON_INSTALL}
endif

ifeq (${HAVE_RUBY},1)
ALL += ${RUBY_BUILD}
SWIG_INSTALL += ${RUBY_INSTALL}
endif
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
	$(RM) -r build${ARCH}/doc/api
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
	rsync -a --cvs-exclude build${ARCH}/doc/api dist/$(DIST_SRC)/doc
	cd dist; zip -r -q $(SRC_ZIP) $(DIST_SRC)
	cd dist; tar --gzip -cf $(SRC_TGZ) $(DIST_SRC)
	$(RM) -r dist/$(DIST_SRC)

_install: _build ${SWIG_INSTALL}
	@$(MKINSTALLDIRS) "$(DESTDIR)$(libdir)"
	$(INSTALL_DATA) $(LIBRETS_LIB) "$(DESTDIR)$(libdir)"
	@$(MKINSTALLDIRS) "$(DESTDIR)$(includedir)/librets"

	$(INSTALL_DATA) $(LIBRETS_INC_FILE) "$(DESTDIR)$(includedir)"
	@list='$(LIBRETS_INC_FILES)'; for p in $$list; do \
	  $(INSTALL_DATA) "$$p" "$(DESTDIR)$(includedir)/librets"; \
	done

	@$(MKINSTALLDIRS) "$(DESTDIR)$(bindir)"
	$(INSTALL_PROGRAM) $(top_srcdir)/librets-config "$(DESTDIR)$(bindir)"

_uninstall: 
	$(RM) $(DESTDIR)$(libdir)/$(LIBRETS_A)
	$(RM) $(DESTDIR)$(includedir)/$(LIBRETS_H)
	$(RM) -r $(DESTDIR)$(includedir)/librets
	$(RM) $(DESTDIR)$(bindir)/librets-config

_clean:
	$(RM) -r build${ARCH} 

_distclean: _clean
	$(RM) $(DISTCLEAN_FILES)
	$(RM) -r autom4te.cache
	$(RM) Makefile
	${RM} -r dist
	${RM} -r distwin

_veryclean: _distclean

_test: cppunit prepare $(LIBRETS_TEST_EXE)
	./$(LIBRETS_TEST_EXE) $(top_srcdir)

_test-network: cppunit prepare $(LIBRETS_NETTEST_EXE) ${LIBRETS_NETTEST_HTTPSERVER}
	@./$(LIBRETS_NETTEST_EXE) $(top_srcdir) --url http://localhost:4444/rets/login || \
	(echo You must enable httpServer. && \
	echo You can do this by typeing: && \
	echo make run-server && \
	echo in another shell.)

_test-ssl: cppunit prepare $(LIBRETS_NETTEST_EXE) ${LIBRETS_NETTEST_HTTPSERVER}
	@./$(LIBRETS_NETTEST_EXE) $(top_srcdir) --url https://localhost:4444/rets/login || \
	(echo You must enable httpServer. && \
	echo You can do this by typeing: && \
	echo make run-ssl-server && \
	echo in another shell.)

_run_httpServer: ${LIBRETS_NETTEST_HTTPSERVER} 
	$(JAVAC)  ${LIBRETS_NETTEST_SRC_DIR}/httpServer.java -d ${LIBRETS_NETTEST_BIN_DIR}
	java -cp ${LIBRETS_NETTEST_BIN_DIR} httpServer --resource=${LIBRETS_NETTEST_SRC_DIR}/resources --port=4444 
	
_run_SSLServer: ${LIBRETS_NETTEST_HTTPSERVER} 
	$(JAVAC)  ${LIBRETS_NETTEST_SRC_DIR}/httpServer.java -d ${LIBRETS_NETTEST_BIN_DIR}
	$(RM) myKeyStore
	keytool -selfcert -keystore myKeyStore -keypass 123456 -storepass 123456 -genkey \
		-keyalg RSA -dname "cn=librets, ou=CRT, o=NAR, l=Chicago, st=IL, c=US" -alias mycert
	java -cp ${LIBRETS_NETTEST_BIN_DIR} -Djavax.net.ssl.keyStore=myKeyStore \
		-Djavax.net.ssl.keyStorePassword=123456 \
		httpServer --resource=${LIBRETS_NETTEST_SRC_DIR}/resources --port=4444 --ssl
	
_maintainer-clean: _veryclean
	$(RM) configure

# Windows cygwin/mingw only
ifeq ($(shell perl -e 'use Config; print $$Config{osname};'), MSWin32)

COMPILER_VER	= ${shell ${CXX} -dumpversion | ${PERL} -e 'foreach (<STDIN>) {s/([1-9]).([1-9]).*/\1\2/;print}'}
DISTDIR		= distwin/${DIST_BIN}
DISTZIP		= librets-${VERSION}${ARCH}.zip

_distwin_stage: _build _dist-prepare _dist-cpp _dist-csharp _dist-java _dist-perl _dist-ruby

_distwin: _distwin_stage _doc-api
	-mkdir dist
	-rsync --recursive --delete ${BUILD}/doc/api/html ${DISTDIR}/doc/api
	-rsync --cvs-exclude --recursive --delete project/librets/include ${DISTDIR}
	cd distwin; zip -rq $(DISTZIP) ./librets-${VERSION}${ARCH}
	cp -f distwin/${DISTZIP} dist

_dist-prepare: 
	-mkdir -p ${DISTDIR}
	-mkdir -p ${DISTDIR}/doc
	-mkdir -p ${DISTDIR}/dotnet
	-mkdir -p ${DISTDIR}/dotnet-mt
	-mkdir -p ${DISTDIR}/java
	-mkdir -p ${DISTDIR}/java-mt
	-mkdir -p ${DISTDIR}/lib
	-mkdir -p ${DISTDIR}/perl
	-mkdir -p ${DISTDIR}/perl-mt
	-mkdir -p ${DISTDIR}/ruby
	-mkdir -p ${DISTDIR}/ruby-mt

_dist-cpp:
	cp -u $(BUILD)/librets/lib/librets.a $(DISTDIR)/lib/librets-mingw${COMPILER_VER}${LIBRETS_MT}${ARCH}.a

_dist-csharp:
	-cp -u ${BUILD}/swig/csharp/*.dll $(DISTDIR)/dotnet${LIBRETS_MT}
	-cp -u ${BUILD}/swig/csharp/Metadata.exe $(DISTDIR)/dotnet${LIBRETS_MT}
	-cp -u ${BUILD}/swig/csharp/Search.exe $(DISTDIR)/dotnet${LIBRETS_MT}
	-cp -u ${BUILD}/swig/csharp/RawSearch.exe $(DISTDIR)/dotnet${LIBRETS_MT}
	-cp -u ${BUILD}/swig/csharp/Update.exe $(DISTDIR)/dotnet${LIBRETS_MT}

_dist-java:
	-cp -u ${BUILD}/swig/java/*.jar $(DISTDIR)/java${LIBRETS_MT}
	-cp -u ${BUILD}/swig/java/*.dll $(DISTDIR)/java${LIBRETS_MT}

_dist-perl:
	-rsync --recursive --delete ${BUILD}/swig/perl/blib/arch/auto/librets/ $(DISTDIR)/perl${LIBRETS_MT}
	-cp -u ${BUILD}/swig/perl/librets.pm $(DISTDIR)/perl${LIBRETS_MT}
	@echo Place librets.pm into your site/lib directory > $(DISTDIR)/perl${LIBRETS_MT}/README
	@echo Create the directory site/lib/auto/librets >> $(DISTDIR)/perl${LIBRETS_MT}/README
	@echo and place the other files there. >> $(DISTDIR)/perl${LIBRETS_MT}/README

_dist-ruby:
	-cp -u ${BUILD}/swig/ruby/*.so $(DISTDIR)/ruby${LIBRETS_MT}
	-cp -u ${BUILD}/swig/ruby/librets.rb $(DISTDIR)/ruby${LIBRETS_MT}

else
_distwin:

endif 

# Cancel built-in rules
%.o: %.cpp
%.o: %.c
%.o: %.d

ifeq ($(USE_DEPENDS),1)
-include $(ALL_DEPENDS)
endif

.PHONY: all debug build doc doc-api \
	install install-bin install-data install-config no-cppunit \
	_all _debug _build _doc _doc-api _clean _distclean _veryclean \
	_maintainer-clean test _test test-network _test-network _distwin

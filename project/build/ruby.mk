###
# ruby of swig
#

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
	CC="${CC}" CXX="${CXX}" CFLAGS="${CFLAGS}" CXXFLAGS="${CXXFLAGS}" \
	${RUBY} -C ${RUBY_OBJ_DIR} ${RUBY_EXTCONF_RB} 		\
	--with-librets-config=${SWIG_LIBRETS_CONFIG} \
        --with-librets-cflags="`${SWIG_LIBRETS_CONFIG} --cflags`" \
        --with-librets-libs="`${SWIG_LIBRETS_CONFIG} --libs`" \
	--with-swig-dir=${SWIG_DIR}

${RUBY_DLL}: ${RUBY_MAKEFILE} ${LIBRETS_LIB}
	${MAKE} CC="${CC}" CXX="${CXX}" LDFLAGS="${LDFLAGS}" -C ${RUBY_OBJ_DIR}
	
${RUBY_INSTALL}: ${RUBY_DLL}
	${MAKE} -C ${RUBY_OBJ_DIR} install


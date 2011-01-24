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
	cp ${RUBY_SRC_DIR}/*.rb ${RUBY_OBJ_DIR}
	cp ${RUBY_SRC_DIR}/librets_ruby.h ${RUBY_OBJ_DIR}
	CC="${CC}" CXX="${CXX}" CFLAGS="${CFLAGS}" CXXFLAGS="${CXXFLAGS}" \
	${RUBY} -C ${RUBY_OBJ_DIR} ${RUBY_EXTCONF_RB} 		\
	--with-librets-config=${SWIG_LIBRETS_CONFIG} \
        --with-librets-cflags="`${SWIG_LIBRETS_CONFIG} --cflags`" \
        --with-librets-libs="`${SWIG_LIBRETS_CONFIG} --libs`" \
	--with-swig-dir=${SWIG_DIR}

ifneq (${SWIG_OSNAME}, MSWin32)
########
#
# Not Windows/MinGW
#

${RUBY_DLL}: ${RUBY_MAKEFILE} ${LIBRETS_LIB}
	${MAKE} CC="${CC}" CXX="${CXX}" LDFLAGS="${LDFLAGS}" -C ${RUBY_OBJ_DIR}
	
${RUBY_INSTALL}: ${RUBY_DLL}
	${MAKE} -C ${RUBY_OBJ_DIR} install

else
########
#
# Windows/MinGW
#


RUBY_ARCH		= ${shell ruby -e "require 'rbconfig'; puts Config::CONFIG['arch']"}
RUBY_ARCH_DIR		= ${shell ruby -e "require 'rbconfig'; puts Config::CONFIG['archdir']"}
RUBY_CXX_FLAGS		= -DHAVE_STRUCT_TIMESPEC \
				-DHAVE_STRUCT_TIMEZONE \
				-DHAVE_CONFIG_H \
				-DTARGET_UNIX \
				`${SWIG_LIBRETS_CONFIG} --cflags`
RUBY_DLL		= ${RUBY_OBJ_DIR}/librets_native.so
RUBY_HDR_DIR		= ${shell ruby -e "require 'rbconfig'; puts Config::CONFIG['rubyhdrdir']"}
RUBY_LIB_DIR		= ${shell ruby -e "require 'rbconfig'; puts Config::CONFIG['libdir']"}
RUBY_RUBYLIB_DIR	= ${shell ruby -e "require 'rbconfig'; puts Config::CONFIG['rubylibprefix']"}
RUBY_LIB		= ${shell ruby -e "require 'rbconfig'; puts Config::CONFIG['RUBY_SO_NAME']"}

${RUBY_OBJ_DIR}/librets_wrap.o: ${RUBY_OBJ_DIR}/librets_wrap.cpp ${RUBY_MAKEFILE}
	${CXX} ${RUBY_CXX_FLAGS} 		\
		-I${LIBRETS_INC_DIR} 		\
		-I${SWIG_DIR} 			\
		${BOOST_CFLAGS} 		\
		-I${RUBY_HDR_DIR} 		\
		-I${RUBY_HDR_DIR}/${RUBY_ARCH} 	\
		-I${RUBY_HDR_DIR}/ruby/backward	\
		-c $< -o $@


${RUBY_DLL}: ${RUBY_MAKEFILE} ${LIBRETS_LIB} ${RUBY_OBJ_DIR}/librets_wrap.o
	${CXX} -o ${RUBY_DLL} 			\
		-shared 			\
		-static-libgcc 			\
		-static-libstdc++ 		\
		${RUBY_OBJ_DIR}/librets_wrap.o 	\
		${SWIG_LIBRETS_LIBS} 		\
		-L${RUBY_RUBYLIB_DIR}		\
		-L${RUBY_LIB_DIR}		\
		-l${RUBY_LIB}
	
${RUBY_INSTALL}: ${RUBY_DLL}
	${MAKE} -C ${RUBY_OBJ_DIR} install

endif

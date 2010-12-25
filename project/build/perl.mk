###
# perl of swig - Perl isn't completely implemented and currently won't build, so this section is commented out.
#

PERL_BUILD		= ${PERL_DLL}

PERL_CFLAGS		= `${top_srcdir}/librets-config-inplace --cflags`
PERL_DLL_DIR		= ${PERL_OBJ_DIR}/blib/arch/auto/librets
PERL_DLL		= ${PERL_DLL_DIR}/librets.${DLL}
# PERL_INCLUDES		=`fgrep PERL_INC ${PERL_MAKEFILE} | head -1 | cut -f 2 -d =`
PERL_LIB		= ${shell perl -e 'use Config; print $$Config{libperl};'}
PERL_LIB_DIR		= ${shell perl -e 'use Config; $$libdir = $$Config{archlibexp}; \
				$$libdir=~s/(^[a-zA-Z]):/\/$$1/;$$libdir=~s/\\/\//g;print $$libdir;'}/CORE
PERL_INCLUDES		= ${PERL_LIB_DIR}
PERL_INSTALL		= perl_install
PERL_LDFLAGS		= `${top_srcdir}/librets-config-inplace --libs`
PERL_MAKEFILE		= ${PERL_OBJ_DIR}/Makefile
PERL_MAKEFILE_PL	= Makefile.PL
PERL_MAKEFILE_PL_MINGW	= Makefile.mingw.PL
PERL_MANIFEST		= MANIFEST
PERL_OBJ_DIR		= ${SWIG_OBJ_DIR}/perl
PERL_SEARCH_EXAMPLE	= search.pl
PERL_SRC_DIR		= ${SWIG_DIR}/perl
PERL_WRAP 		= ${PERL_OBJ_DIR}/librets_wrap.cpp

${PERL_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -perl -o ${PERL_WRAP} \
	-outdir ${PERL_OBJ_DIR} ${SWIG_DIR}/librets.i

ifeq (${SWIG_OSNAME}, MSWin32)

${PERL_OBJ_DIR}/${PERL_MAKEFILE_PL}: ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL_MINGW}
	cp ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL_MINGW} ${PERL_OBJ_DIR}/${PERL_MAKEFILE_PL}
	cp ${PERL_SRC_DIR}/${PERL_MANIFEST} ${PERL_OBJ_DIR}
	cp ${PERL_SRC_DIR}/${PERL_SEARCH_EXAMPLE} ${PERL_OBJ_DIR}

${PERL_MAKEFILE}: ${PERL_WRAP} ${PERL_OBJ_DIR}/${PERL_MAKEFILE_PL}
	cd ${PERL_OBJ_DIR}; perl ${PERL_MAKEFILE_PL} \
		CC="${CXX}" \
		CCFLAGS="${PERL_CFLAGS}" \
		LD="${SWIG_LINK}" \
		LDFLAGS="${PERL_LDFLAGS}" \
		LDDLFLAGS="-DFOO" \
		OPTIMIZE="-DFOOBAR" \
		LIBS="${PERL_LDFLAdGS}"

${PERL_OBJ_DIR}/librets_wrap.obj: ${PERL_OBJ_DIR}/librets_wrap.cpp ${PERL_MAKEFILE}
	${CXX} ${PERL_CXX_FLAGS} -I${LIBRETS_INC_DIR} -I${SWIG_DIR} ${BOOST_CFLAGS} -I${PERL_INCLUDES} -c $< -o $@


${PERL_DLL}: ${PERL_MAKEFILE} ${LIBRETS_LIB} ${PERL_OBJ_DIR}/librets_wrap.obj
	[ -d ${PERL_DLL_DIR} ] || mkdir -p ${PERL_DLL_DIR}
	${SWIG_LINK} -o ${PERL_DLL} ${PERL_OBJ_DIR}/librets_wrap.obj ${SWIG_LIBRETS_LIBS} ${PERL_LIB_DIR}/${PERL_LIB}
#	cd ${PERL_OBJ_DIR}; (unset MAKEFLAGS; nmake || nmake)
	
${PERL_INSTALL}: ${PERL_DLL} ${PERL_MAKEFILE}
	cd ${PERL_OBJ_DIR}; (unset MAKEFLAGS; nmake install)
else

${PERL_OBJ_DIR}/${PERL_MAKEFILE_PL}: ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL}
	cp ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL} ${PERL_OBJ_DIR}/${PERL_MAKEFILE_PL}
	cp ${PERL_SRC_DIR}/${PERL_MANIFEST} ${PERL_OBJ_DIR}
	cp ${PERL_SRC_DIR}/${PERL_SEARCH_EXAMPLE} ${PERL_OBJ_DIR}

${PERL_MAKEFILE}: ${PERL_WRAP} ${PERL_OBJ_DIR}/${PERL_MAKEFILE_PL}
	cd ${PERL_OBJ_DIR}; perl ${PERL_MAKEFILE_PL}

${PERL_DLL}: ${PERL_MAKEFILE} ${LIBRETS_LIB}
	${MAKE} -C ${PERL_OBJ_DIR} || ${MAKE} -C ${PERL_OBJ_DIR}
	
${PERL_INSTALL}: ${PERL_DLL} ${PERL_MAKEFILE}
	cd ${PERL_OBJ_DIR}; ${MAKE} install
endif


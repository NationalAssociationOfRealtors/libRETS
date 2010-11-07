###
# perl of swig - Perl isn't completely implemented and currently won't build, so this section is commented out.
#

PERL_BUILD		= ${PERL_DLL}

PERL_DLL		= ${PERL_OBJ_DIR}/blib/arch/auto/librets/librets.${DLL}
PERL_INSTALL		= perl_install
PERL_MAKEFILE		= ${PERL_OBJ_DIR}/Makefile
PERL_MAKEFILE_PL	= Makefile.PL
PERL_MAKEFILE_PL_MINGW	= Makefile.mingw.PL
PERL_OBJ_DIR		= ${SWIG_OBJ_DIR}/perl
PERL_SRC_DIR		= ${SWIG_DIR}/perl
PERL_WRAP 		= ${PERL_OBJ_DIR}/librets_wrap.cpp

${PERL_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -perl -o ${PERL_WRAP} \
	-outdir ${PERL_OBJ_DIR} ${SWIG_DIR}/librets.i

ifeq (${SWIG_OSNAME}, MSWin32)

${PERL_MAKEFILE}: ${PERL_WRAP} ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL_MINGW}
	cp ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL_MINGW} ${PERL_OBJ_DIR}/${PERL_MAKEFILE_PL}
	cd ${PERL_OBJ_DIR}; perl ${PERL_MAKEFILE_PL}


${PERL_DLL}: ${PERL_MAKEFILE} ${LIBRETS_LIB}
	cd ${PERL_OBJ_DIR}; (unset MAKEFLAGS; nmake || nmake)
	
${PERL_INSTALL}: ${PERL_DLL} ${PERL_MAKEFILE}
	cd ${PERL_OBJ_DIR}; (unset MAKEFLAGS; nmake install)
else

${PERL_MAKEFILE}: ${PERL_WRAP} ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL}
	cp ${PERL_SRC_DIR}/${PERL_MAKEFILE_PL} ${PERL_OBJ_DIR}
	cd ${PERL_OBJ_DIR}; perl ${PERL_MAKEFILE_PL}

${PERL_DLL}: ${PERL_MAKEFILE} ${LIBRETS_LIB}
	${MAKE} -C ${PERL_OBJ_DIR} || ${MAKE} -C ${PERL_OBJ_DIR}
	
${PERL_INSTALL}: ${PERL_DLL} ${PERL_MAKEFILE}
	cd ${PERL_OBJ_DIR}; ${MAKE} install
endif


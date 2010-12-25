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
SWIG_OBJ_DIR		= ${BUILD}/swig
SWIG_OSNAME		= $(shell perl -e 'use Config; print $$Config{osname};')

SWIG_BRIDGE_CFLAGS	= `${SWIG_LIBRETS_CONFIG} --cflags` ${CFLAGS}
SWIG_BRIDGE_H		= ${SWIG_DIR}/librets_bridge.h
SWIG_BRIDGE_SRC		= ${SWIG_DIR}/librets_bridge.cpp
SWIG_BRIDGE_OBJ		= ${SWIG_OBJ_DIR}/librets_bridge.o

DLL			= so
SWIG_LINK		= ${CXX} -shared

ifeq (${SWIG_OSNAME}, darwin)
SWIG_LINK		= ${CXX} -bundle -undefined suppress -flat_namespace 
else ifeq (${SWIG_OSNAME}, MSWin32)
DLL			= dll
SWIG_LINK		= ${CXX} -static -mdll
endif

${SWIG_BRIDGE_OBJ}: ${SWIG_BRIDGE_SRC} ${SWIG_BRIDGE_H}
	${CXX} ${SWIG_BRIDGE_CFLAGS} -I${LIBRETS_INC_DIR}  -I${SWIG_DIR} ${BOOST_CFLAGS} -c $< -o $@
	
###
# dotnet
#
ifeq (${HAVE_DOTNET},1)
include ${top_srcdir}/project/build/dotnet.mk
endif

###
# java
#
ifeq (${HAVE_JAVA},1)
include ${top_srcdir}/project/build/java.mk
endif

###
# php 
#
ifeq (${HAVE_PHP},1)
ifneq (${SWIG_OSNAME}, MSWin32)
include ${top_srcdir}/project/build/php.mk
endif
endif

###
# perl 
#
ifeq (${HAVE_PERL},1)
include ${top_srcdir}/project/build/perl.mk
endif

###
# python
#
ifeq (${HAVE_PYTHON},1)
ifneq (${SWIG_OSNAME}, MSWin32)
include ${top_srcdir}/project/build/python.mk
endif
endif

###
# ruby 
#
ifeq (${HAVE_RUBY},1)
include ${top_srcdir}/project/build/ruby.mk
endif

endif

###
# python of swig
#

PYTHON_BUILD		= ${PYTHON_DLL}

PYTHON_DLL		= ${PYTHON_OBJ_DIR}/_librets.${DLL}
PYTHON_INSTALL		= python_install
PYTHON_OBJ_DIR		= ${SWIG_OBJ_DIR}/python
PYTHON_SRC_DIR		= ${SWIG_DIR}/python
PYTHON_WRAP 		= ${PYTHON_OBJ_DIR}/librets_wrap.cpp

${PYTHON_WRAP}: ${SWIG_FILES}  ${PYTHON_SRC_DIR}/setup.py
	@cp ${PYTHON_SRC_DIR}/*.py ${PYTHON_OBJ_DIR}
	${SWIG} -c++ -python -o ${PYTHON_WRAP} \
	-outdir ${PYTHON_OBJ_DIR} ${SWIG_DIR}/librets.i

ifneq (${SWIG_OSNAME}, MSWin32)
${PYTHON_DLL}: ${PYTHON_WRAP} ${LIBRETS_LIB}
	cd ${PYTHON_OBJ_DIR} ; \
	CC="${CC}" CXX="${CXX}" CFLAGS="${CFLAGS}" CXXFLAGS="${CXXFLAGS}" \
	${PYTHON} setup.py build  
else
PYTHON_CXX_FLAGS	= -mcygwin -mdll \
				`${SWIG_LIBRETS_CONFIG} --cflags`
PYTHON_DLL		= ${PYTHON_OBJ_DIR}/librets_native.so
PYTHON_HDR_DIR		= ${shell python -c "import sysconfig; print sysconfig.get_config_vars()['INCLUDEPY']" | ${FORWARDSLASH}}
PYTHON_LIB_DIR		= ${shell python -c "import sysconfig; print sysconfig.get_config_vars()['LIBDEST']" | ${FORWARDSLASH}}
PYTHON_LIB		= ${shell python -c "import sysconfig; print sysconfig.get_config_vars()['INCLUDEPY']" | ${FORWARDSLASH}}

FORWARDSLASH		= perl -e 'foreach (<STDIN>) {s/\\/\//g; print}'

${PYTHON_DLL}: ${PYTHON_WRAP} ${LIBRETS_LIB} 
	cd ${PYTHON_OBJ_DIR} ; \
	echo ${VERSION} > setup.ini; \
	echo ${shell ${SWIG_LIBRETS_CONFIG} --libs} >> setup.ini; \
	echo ${shell ${SWIG_LIBRETS_CONFIG} --cflags} >> setup.ini; \
	echo ${SWIG_BRIDGE_SRC} >> setup.ini; \
	CC="${CC}" \
	CXX="${CXX}" \
	CFLAGS="${CFLAGS}" \
	OPT="-DFOO" \
	cmd.exe /c "${PYTHON} setup.py build --build-lib=. --compiler=cygwin"

${PYTHON_OBJ_DIR}/librets_wrap.o: ${PYTHON_OBJ_DIR}/librets_wrap.cpp
	${CXX} ${PYTHON_CXX_FLAGS} 		\
		-I${LIBRETS_INC_DIR} 		\
		-I${SWIG_DIR} 			\
		${BOOST_CFLAGS} 		\
		-I${PYTHON_LIB_DIR} 		\
		-I${PYTHON_HDR_DIR} 		\
		-c $< -o $@


${PYTHON_DLLx}: ${PYTHON_MAKEFILE} ${LIBRETS_LIB} ${PYTHON_OBJ_DIR}/librets_wrap.o
	${CXX} -o ${PYTHON_DLL} 			\
		-shared 			\
		-static-libgcc 			\
		-static-libstdc++ 		\
		${PYTHON_OBJ_DIR}/librets_wrap.o 	\
		${SWIG_LIBRETS_LIBS} 		
	
endif
	
${PYTHON_INSTALL}: ${PYTHON_DLL}
	cd ${PYTHON_OBJ_DIR}; ${PYTHON} setup.py install


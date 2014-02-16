###
# python3 of swig
#

PYTHON3_BUILD		= ${PYTHON3_DLL}

PYTHON3_DLL		= ${PYTHON3_OBJ_DIR}/_librets.${DLL}
PYTHON3_INSTALL		= python3_install
PYTHON3_OBJ_DIR		= ${SWIG_OBJ_DIR}/python3
PYTHON3_SRC_DIR		= ${SWIG_DIR}/python3
PYTHON3_WRAP 		= ${PYTHON3_OBJ_DIR}/librets_wrap.cpp

${PYTHON3_WRAP}: ${SWIG_FILES}  ${PYTHON3_SRC_DIR}/setup.py
	@cp ${PYTHON3_SRC_DIR}/*.py ${PYTHON3_OBJ_DIR}
	${SWIG} -c++ -python -py3 -o ${PYTHON3_WRAP} \
	-outdir ${PYTHON3_OBJ_DIR} ${SWIG_DIR}/librets.i
	sed 's/PyBaseObject_Type\./((PyTypeObject*) data->newargs)->/' \
	${PYTHON3_OBJ_DIR}/librets_wrap.cpp > ${PYTHON3_OBJ_DIR}/librets_wrap_new.cpp
	mv ${PYTHON3_OBJ_DIR}/librets_wrap_new.cpp ${PYTHON3_OBJ_DIR}/librets_wrap.cpp

ifneq (${SWIG_OSNAME}, MSWin32)
${PYTHON3_DLL}: ${PYTHON3_WRAP} ${LIBRETS_LIB}
	cd ${PYTHON3_OBJ_DIR} ; \
	CC="${CC}" CXX="${CXX}" CFLAGS="${CFLAGS}" CXXFLAGS="${CXXFLAGS}" \
	${PYTHON3} setup.py build --build-lib=.
else
PYTHON3_CXX_FLAGS	= -mcygwin -mdll \
				`${SWIG_LIBRETS_CONFIG} --cflags`
PYTHON3_DLL		= ${PYTHON3_OBJ_DIR}/librets_native.so
PYTHON3_HDR_DIR		= ${shell python3 -c "import sysconfig; print(sysconfig.get_config_vars()['INCLUDEPY'])" | ${FORWARDSLASH}}
PYTHON3_LIB_DIR		= ${shell python3 -c "import sysconfig; print(sysconfig.get_config_vars()['LIBDEST'])" | ${FORWARDSLASH}}
PYTHON3_LIB		= ${shell python3 -c "import sysconfig; print(sysconfig.get_config_vars()['INCLUDEPY'])" | ${FORWARDSLASH}}

FORWARDSLASH		= perl -e 'foreach (<STDIN>) {s/\\/\//g; print}'

${PYTHON3_DLL}: ${PYTHON3_WRAP} ${LIBRETS_LIB}
	cd ${PYTHON3_OBJ_DIR} ; \
	echo ${VERSION} > setup.ini; \
	echo ${shell ${SWIG_LIBRETS_CONFIG} --libs} >> setup.ini; \
	echo ${shell ${SWIG_LIBRETS_CONFIG} --cflags} >> setup.ini; \
	echo ${SWIG_BRIDGE_SRC} >> setup.ini; \
	CC="${CC}" \
	CXX="${CXX}" \
	CFLAGS="${CFLAGS}" \
	OPT="-DFOO" \
	cmd.exe /c "${PYTHON3} setup.py build --build-lib=. --compiler=cygwin"

${PYTHON3_OBJ_DIR}/librets_wrap.o: ${PYTHON3_OBJ_DIR}/librets_wrap.cpp
	${CXX} ${PYTHON3_CXX_FLAGS} 		\
		-I${LIBRETS_INC_DIR} 		\
		-I${SWIG_DIR} 			\
		${BOOST_CFLAGS} 		\
		-I${PYTHON3_LIB_DIR} 		\
		-I${PYTHON3_HDR_DIR} 		\
		-c $< -o $@


${PYTHON3_DLLx}: ${PYTHON3_MAKEFILE} ${LIBRETS_LIB} ${PYTHON3_OBJ_DIR}/librets_wrap.o
	${CXX} -o ${PYTHON3_DLL} 			\
		-shared 			\
		-static-libgcc 			\
		-static-libstdc++ 		\
		${PYTHON3_OBJ_DIR}/librets_wrap.o 	\
		${SWIG_LIBRETS_LIBS}

endif

${PYTHON3_INSTALL}: ${PYTHON3_DLL}
	cd ${PYTHON3_OBJ_DIR}; ${PYTHON3} setup.py install


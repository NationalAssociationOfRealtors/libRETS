###
# python of swig
#

PYTHON_BUILD		= ${PYTHON_DLL}

PYTHON_DLL		= ${PYTHON_OBJ_DIR}/_librets.${DLL}
PYTHON_INSTALL		= python_install
PYTHON_OBJ_DIR		= ${SWIG_OBJ_DIR}/python
PYTHON_SRC_DIR		= ${SWIG_DIR}/python
PYTHON_WRAP 		= ${PYTHON_OBJ_DIR}/librets_wrap.cpp

${PYTHON_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -python -o ${PYTHON_WRAP} \
	-outdir ${PYTHON_OBJ_DIR} ${SWIG_DIR}/librets.i
	@cp ${PYTHON_SRC_DIR}/* ${PYTHON_OBJ_DIR}

${PYTHON_DLL}: ${PYTHON_WRAP} ${LIBRETS_LIB}
	cd ${PYTHON_OBJ_DIR} ; ${PYTHON} setup.py build --build-lib=.
	
${PYTHON_INSTALL}: ${PYTHON_DLL}
	cd ${PYTHON_OBJ_DIR}; ${PYTHON} setup.py install


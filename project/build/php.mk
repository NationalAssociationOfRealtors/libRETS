###
# php of swig
#

PHP_BUILD		= ${PHP_DLL} 

PHP_CXX_FLAGS		= `${SWIG_LIBRETS_CONFIG} --cflags`
PHP_DLL			= ${PHP_OBJ_DIR}/librets.${DLL}
PHP_INCLUDES		= `php-config --includes`
PHP_LDFLAGS		= `php-config --ldflags`
PHP_LIBS		= `php-config --libs`
PHP_OBJ_DIR		= ${SWIG_OBJ_DIR}/php5
PHP_SRC_DIR		= ${SWIG_DIR}/php5
PHP_WRAP 		= ${PHP_OBJ_DIR}/librets_wrap.cpp

${PHP_WRAP}: ${SWIG_FILES} 
	${SWIG} -c++ -php5 -o ${PHP_WRAP} \
	-outdir ${PHP_OBJ_DIR} ${SWIG_DIR}/librets.i

${PHP_DLL}: ${PHP_WRAP} ${PHP_OBJ_DIR}/librets_wrap.o ${SWIG_BRIDGE_OBJ} ${LIBRETS_LIB}
	${SWIG_LINK} -o ${PHP_DLL} ${PHP_OBJ_DIR}/librets_wrap.o ${SWIG_LIBRETS_LIBS} ${SWIG_BRIDGE_OBJ}

${PHP_OBJ_DIR}/librets_wrap.o: ${PHP_OBJ_DIR}/librets_wrap.cpp
	${CXX} ${PHP_CXX_FLAGS} -g -DLIBRETS_VERSION='"$(VERSION)"' -I${LIBRETS_INC_DIR} -I${PHP_SRC_DIR} \
			-I${SWIG_DIR} ${BOOST_CFLAGS} ${PHP_INCLUDES} -c $< -o $@

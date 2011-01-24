###
# java of swig
#

JAVA_BUILD		= ${JAVA_DLL} ${JAVA_OBJ_DIR}/${JAVA_JAR} ${JAVA_EXAMPLES_CLASSES}

JAVA_BRIDGE		= ${JAVA_SRC_DIR}/CppInputStream.java
# delete the next line to enable the streams prototype for Java
JAVA_BRIDGE		= 
JAVA_CLASSES		= ${patsubst ${JAVA_OBJ_DIR}/%.java,${JAVA_OBJ_DIR}/librets/%.class,${JAVA_SOURCES}}
JAVA_CLASSES_UNQUAL	= ${patsubst ${JAVA_OBJ_DIR}/%.java,%.class,${JAVA_SOURCES}}
JAVA_CXX_FLAGS		= `${SWIG_LIBRETS_CONFIG} --cflags`
JAVA_EXAMPLES		= ${wildcard ${JAVA_SRC_DIR}/[a-z]*.java}
JAVA_EXAMPLES_CLASSES	= ${patsubst ${JAVA_SRC_DIR}/%.java,${JAVA_OBJ_DIR}/%.class,${JAVA_EXAMPLES}}
JAVA_JAR		= librets.jar
JAVA_OBJ_DIR		= ${SWIG_OBJ_DIR}/java
JAVA_SOURCES		= ${wildcard ${JAVA_OBJ_DIR}/*.java}
JAVA_SRC_DIR		= ${SWIG_DIR}/java
JAVA_WRAP 		= ${JAVA_OBJ_DIR}/librets_wrap.cpp

ifeq (${SWIG_OSNAME}, darwin)
JAVA_CLASSPATH		= `javaconfig DefaultClasspath`:${JAVA_OBJ_DIR}/${JAVA_JAR}
JAVA_DLL		= ${JAVA_OBJ_DIR}/liblibrets.jnilib
JAVA_DYNAMICLINK	= ${SWIG_LINK}
else
JAVA_CLASSPATH		= ${JAVA_OBJ_DIR}/${JAVA_JAR}
JAVA_DLL		= ${JAVA_OBJ_DIR}/liblibrets.${DLL}
ifeq (${SWIG_OSNAME}, MSWin32)
JAVA_DLL		= ${JAVA_OBJ_DIR}/librets.${DLL}
endif
JAVA_DYNAMICLINK	= ${SWIG_LINK}
endif

${JAVA_WRAP}: ${SWIG_FILES} ${JAVA_BRIDGE}
	${SWIG} -c++ -java -package librets -o ${JAVA_WRAP} \
	-outdir ${JAVA_OBJ_DIR} ${SWIG_DIR}/librets.i
	@echo ${JAVA_BRIDGE} ${JAVA_OBJ_DIR}
	${MAKE} ${JAVA_OBJ_DIR}/${JAVA_JAR}

${JAVA_DLL}: ${JAVA_WRAP} ${JAVA_OBJ_DIR}/librets_wrap.o ${SWIG_BRIDGE_OBJ} ${LIBRETS_LIB}
	${JAVA_DYNAMICLINK} -o ${JAVA_DLL} ${JAVA_OBJ_DIR}/librets_wrap.o ${SWIG_LIBRETS_LIBS} ${SWIG_BRIDGE_OBJ}

${JAVA_OBJ_DIR}/librets_wrap.o: ${JAVA_OBJ_DIR}/librets_wrap.cpp
	${CXX}  ${JAVA_CXX_FLAGS} -I${LIBRETS_INC_DIR} -I${SWIG_DIR} ${BOOST_CFLAGS} ${JAVA_INCLUDES} -c $< -o $@
	

ifneq (${SWIG_OSNAME}, MSWin32)
########
#
# Not Windows/MinGW
#
${JAVA_CLASSES}: ${JAVA_WRAP} ${JAVA_SOURCES}
	${JAVAC} -d ${JAVA_OBJ_DIR} ${JAVA_BRIDGE} ${JAVA_SOURCES} 

${JAVA_EXAMPLES_CLASSES}: ${JAVA_EXAMPLES} ${JAVA_OBJ_DIR}/${JAVA_JAR}
	${JAVAC} -classpath ${JAVA_CLASSPATH} -d ${JAVA_OBJ_DIR} ${JAVA_EXAMPLES}

${JAVA_OBJ_DIR}/${JAVA_JAR}: ${JAVA_CLASSES}
	cd ${JAVA_OBJ_DIR}; ${JAR} -cvf ${JAVA_JAR} librets || \
					${RM} ${JAVA_OBJ_DIR}/${JAVA_JAR}
else
########
#
# Windows/MinGW
#
${JAVA_CLASSES}: ${JAVA_WRAP} ${JAVA_SOURCES}
	${JAVAC} -d ${shell echo ${JAVA_OBJ_DIR} | ${BACKSLASH}} \
                ${shell echo ${JAVA_BRIDGE} | ${BACKSLASH}} \
                ${shell echo ${JAVA_SOURCES} | ${BACKSLASH}} 

${JAVA_EXAMPLES_CLASSES}: ${JAVA_EXAMPLES} ${JAVA_OBJ_DIR}/${JAVA_JAR}
	${JAVAC} -classpath ${shell echo ${JAVA_CLASSPATH} | ${BACKSLASH}} \
		-d ${shell echo ${JAVA_OBJ_DIR} | ${BACKSLASH}} \
                ${shell echo ${JAVA_EXAMPLES} | ${BACKSLASH}} 

${JAVA_OBJ_DIR}/${JAVA_JAR}: ${JAVA_CLASSES}
	cd ${JAVA_OBJ_DIR}; ${JAR} -cvf ${shell echo ${JAVA_JAR} | ${BACKSLASH}} librets || \
					${RM} ${JAVA_OBJ_DIR}/${JAVA_JAR}
endif


NODE_BUILD = ${NODE_EXT}

NODE_EXT = ${NODE_OBJ_DIR}/build/Release/librets.node

NODE_OBJ_DIR = ${SWIG_OBJ_DIR}/node.js
NODE_WRAP = ${NODE_OBJ_DIR}/librets_wrap.cpp
NODE_SRC_DIR = ${SWIG_DIR}/node.js
NODE_BINDING = ${NODE_OBJ_DIR}/binding.gyp
NODE_MAKEFILE = ${NODE_OBJ_DIR}/build/Makefile

${NODE_WRAP}: ${SWIG_FILES}
	${SWIG} -c++ -javascript -node -o ${NODE_WRAP} \
	-outdir ${NODE_OBJ_DIR} ${SWIG_DIR}/librets.i

${NODE_BINDING}: ${NODE_SRC_DIR}/binding.gyp
	cp ${NODE_SRC_DIR}/binding.gyp ${NODE_OBJ_DIR}

${NODE_MAKEFILE}: ${NODE_BINDING} ${NODE_OBJ_DIR}/librets_wrap.cpp
	cd ${NODE_OBJ_DIR} && ${NODEGYP} configure

${NODE_EXT}: ${NODE_MAKEFILE}
	cd ${NODE_OBJ_DIR} && ${NODEGYP} build

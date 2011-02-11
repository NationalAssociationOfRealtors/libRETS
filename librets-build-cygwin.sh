#! /bin/sh

# This is a sample configuration script that I use on OS X for
# librets development. It can be customized for your own
# environment.

PATH="/cygdrive/c/librets/vendor/mingw-${TARGET_CPU}/bin:${JAVA_HOME}/bin:$PATH" \
AR="${AR}" \
CC="${CC}" \
CXX="${CXX}" \
CPPFLAGS="-I/cygdrive/c/librets/vendor/mingw-${TARGET_CPU}/include \
                -L/cygdrive/c/librets/vendor/mingw-${TARGET_CPU}/lib" \
 ./configure    --enable-default-search-path="/cygdrive/c/librets/vendor/mingw-${TARGET_CPU} /usr /usr/include" \
 		--enable-examples               	\
                --enable-debug                  	\
                --enable-depends                	\
		--with-java-prefix=${JAVA_HOME}/include \
		--with-platform=${TARGET_CPU}

make clean
make _distwin_stage

PATH="/cygdrive/c/librets/vendor/mingw-${TARGET_CPU}/bin:${JAVA_HOME}/bin:$PATH" \
AR="${AR}" \
CC="${CC}" \
CXX="${CXX}" \
CPPFLAGS="-I/cygdrive/c/librets/vendor/mingw-${TARGET_CPU}/include \
                -L/cygdrive/c/librets/vendor/mingw-${TARGET_CPU}/lib" \
 ./configure    --enable-default-search-path="/cygdrive/c/librets/vendor/mingw-${TARGET_CPU} /usr /usr/include" \
 		--enable-examples               	\
                --enable-debug                  	\
                --enable-depends                	\
                --enable-thread-safe                    \
		--with-java-prefix=${JAVA_HOME}/include \
		--with-platform=${TARGET_CPU}

make clean
make _distwin_stage

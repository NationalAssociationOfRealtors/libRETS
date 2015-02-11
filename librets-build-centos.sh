#! /bin/sh
#
# This builds libRETS on CentOS 7

sudo yum install --assumeyes antlr-tool autoconf boost-devel expat-devel libcurl-devel gcc-c++ swig python-devel

# This is a sample configuration script that I use on FreeBSD for
# librets development. It can be customized for your own
# environment.

./autogen.sh 

CPPFLAGS="-L/usr/local/lib -I/usr/local/include"	\
 ./configure    --enable-examples               	\
                --enable-debug                  	\
                --enable-depends                	\
                --enable-maintainer-documentation	\
                --enable-shared_dependencies		


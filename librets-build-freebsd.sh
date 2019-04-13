#! /bin/sh

# This is a sample configuration script that I use on FreeBSD for
# librets development. It can be customized for your own
# environment.

CPPFLAGS="-L/usr/local/lib -I/usr/local/include"	\
 ./configure    --enable-examples               	\
                --enable-debug                  	\
                --enable-depends                	\
		--enable-maintainer-documentation	\
                --with-expat-prefix=/usr/local  	\
                --with-boost-prefix=/usr/local		\
		--with-java-prefix=/usr/local/openjdk6/include \
		--prefix=/tmp/librets


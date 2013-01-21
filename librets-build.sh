#! /bin/sh

# This is a sample configuration script that I use on OS X for
# librets development. It can be customized for your own
# environment.

CPPFLAGS="-L/opt/local/lib -I/opt/local/include"	\
 ./configure    --enable-examples               	\
                --enable-debug                  	\
                --enable-depends                	\
		--enable-maintainer-documentation	\
                --with-expat-prefix=/opt/local  	\
		--with-java-prefix=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.8.sdk/System/Library/Frameworks/JavaVM.framework/Versions/A/Headers/ \
                --with-boost-prefix=/opt/local		\
                --with-snk-file=no                      \
		--prefix=/tmp/librets


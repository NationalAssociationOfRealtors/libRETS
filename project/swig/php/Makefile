default: all

all: php_librets.so


php_librets.so: librets_wrap.o librets_wrap.cpp
	g++ -shared -fpic -o php_librets.so librets_wrap.o ../../../build/librets/lib/librets.a `../../../librets-config-inplace --libs`

librets_wrap.o: librets_wrap.cpp
	g++ -fpic -I../../librets/include `php-config --includes` -c librets_wrap.cpp

clean:
	rm -f php_librets.so librets_wrap.o

distclean: clean
	rm -f librets.php librets_wrap.cpp php_librets.h

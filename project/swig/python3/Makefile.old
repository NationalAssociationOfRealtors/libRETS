LIBRETS_EXT = _librets.so

all: _librets.so

_librets.so: librets_wrap.cpp
	python setup.py build --build-lib=.

install:
	python setup.py install

clean:
	$(RM) -r build
	$(RM) _librets.so

distclean: clean
	$(RM) librets.py librets.pyc librets_wrap.cpp

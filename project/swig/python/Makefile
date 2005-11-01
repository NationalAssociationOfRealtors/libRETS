LIBRETS_EXT = _librets.so

all: _librets.so

_librets.so:
	python setup.py build --build-lib=.

clean:
	$(RM) -r build
	$(RM) _librets.so

distclean: clean
	$(RM) librets.py librets.pyc

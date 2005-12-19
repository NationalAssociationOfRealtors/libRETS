
GENERATED_SRC = $(wildcard lib/*.cs)

WRAP = 	librets_wrap.cpp

LIBRETS_CONFIG = ../../../librets-config-inplace
LIBRETS_LIBS = `$(LIBRETS_CONFIG) --libs`
CXXFLAGS = `$(LIBRETS_CONFIG) --cflags`
LIBRETS_SHARP_DLL = librets-sharp.dll

osname := $(shell perl -e 'use Config; print $$Config{osname};')

ifeq ($(osname), darwin)
LINK = g++ -bundle -undefined suppress -flat_namespace
else
LINK = g++ -shared
endif

all: librets.so Search.exe Metadata.exe

Search.exe: Search.cs $(LIBRETS_SHARP_DLL) librets.so
	mcs -r:$(LIBRETS_SHARP_DLL) Search.cs

Metadata.exe: Metadata.cs $(LIBRETS_SHARP_DLL) librets.so
	mcs -r:$(LIBRETS_SHARP_DLL) Metadata.cs

$(LIBRETS_SHARP_DLL): librets.so $(GENERATED_SRC)
	mcs -target:library -out:$(LIBRETS_SHARP_DLL) $(GENERATED_SRC)

librets.so: librets_wrap.o
	$(LINK) -o librets.so librets_wrap.o $(LIBRETS_LIBS)

clean:
	$(RM) *.o $(LIBRETS_SHARP_DLL) librets.so Search.exe

distclean: clean
	$(RM) $(GENERATED_SRC) librets_wrap*

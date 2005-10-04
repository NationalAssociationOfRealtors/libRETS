
GENERATED_SRC = \
	LogoutResponse.cs \
	RetsException.cs \
	RetsSession.cs \
	SearchRequest.cs \
	SearchResultSet.cs \
	librets.cs \
	libretsPINVOKE.cs \

WRAP = 	librets_wrap.cpp

LINK = g++ -bundle -undefined suppress -flat_namespace

LIBRETS_CONFIG = ../../../librets-config-inplace
LIBRETS_LIBS = `$(LIBRETS_CONFIG) --libs`
CXXFLAGS = `$(LIBRETS_CONFIG) --cflags`

all: librets.so Search.exe

Search.exe: Search.cs $(GENERATED_SRC) librets.so
	mcs Search.cs $(GENERATED_SRC)

librets.dll: librets.so
	mcs -target:module -out:librets.netmodule $(GENERATED_SRC)

librets.so: librets_wrap.o
	$(LINK) -o librets.so librets_wrap.o $(LIBRETS_LIBS)

clean:
	$(RM) *.o librets.so Search.exe

distclean: clean
	$(RM) $(GENERATED_SRC) librets_wrap*

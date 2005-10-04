
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
LIBRETS_SHARP_DLL = librets-sharp.dll

all: librets.so Search.exe

Search.exe: Search.cs $(LIBRETS_SHARP_DLL) librets.so
	mcs -r:$(LIBRETS_SHARP_DLL) Search.cs

$(LIBRETS_SHARP_DLL): librets.so $(GENERATED_SRC)
	mcs -target:library -out:$(LIBRETS_SHARP_DLL) $(GENERATED_SRC)

librets.so: librets_wrap.o
	$(LINK) -o librets.so librets_wrap.o $(LIBRETS_LIBS)

clean:
	$(RM) *.o $(LIBRETS_SHARP_DLL) librets.so Search.exe

distclean: clean
	$(RM) $(GENERATED_SRC) librets_wrap*

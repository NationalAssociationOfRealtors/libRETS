
GENERATED_SRC = $(wildcard lib/*.cs)

LIBRETS_UNMANAGED_OBJ = librets_wrap.o librets_sharp.o
LIBRETS_UNMANAGED_DLL = librets.so

LIBRETS_MANAGED_SRC = $(GENERATED_SRC) CppInputStream.cs \
	ObjectDescriptorEnumerator.cs TextWriterLogger.cs
LIBRETS_MANAGED_DLL = librets-dotnet.dll

SAMPLE_EXE = Login.exe Search.exe Metadata.exe GetObject.exe

WRAP = 	librets_wrap.cpp

LIBRETS_CONFIG = ../../../librets-config-inplace
LIBRETS_LIBS = `$(LIBRETS_CONFIG) --libs`
CXXFLAGS = `$(LIBRETS_CONFIG) --cflags`

osname := $(shell perl -e 'use Config; print $$Config{osname};')

ifeq ($(osname), darwin)
LINK = g++ -bundle -undefined suppress -flat_namespace
else
LINK = g++ -shared
endif

CSC = mcs -debug

all: $(LIBRETS_MANAGED_DLL) $(SAMPLE_EXE)

Login.exe: Login.cs $(LIBRETS_UNMANAGED_DLL) $(LIBRETS_UNMANAGED_DLL)
	$(CSC) -r:$(LIBRETS_MANAGED_DLL) Login.cs

Search.exe: Search.cs $(LIBRETS_UNMANAGED_DLL) $(LIBRETS_UNMANAGED_DLL)
	$(CSC) -r:$(LIBRETS_MANAGED_DLL) Search.cs

Metadata.exe: Metadata.cs $(LIBRETS_MANAGED_DLL) $(LIBRETS_UNMANAGED_DLL)
	$(CSC) -r:$(LIBRETS_MANAGED_DLL) Metadata.cs

GetObject.exe: GetObject.cs $(LIBRETS_MANAGED_DLL) $(LIBRETS_UNMANAGED_DLL)
	$(CSC) -r:$(LIBRETS_MANAGED_DLL) GetObject.cs

$(LIBRETS_MANAGED_DLL): $(LIBRETS_UNMANAGED_DLL) $(LIBRETS_MANAGED_SRC)
	$(CSC) -target:library -out:$(LIBRETS_MANAGED_DLL) $(LIBRETS_MANAGED_SRC)

$(LIBRETS_UNMANAGED_DLL): $(LIBRETS_UNMANAGED_OBJ)
	$(LINK) -o $(LIBRETS_UNMANAGED_DLL) $(LIBRETS_UNMANAGED_OBJ) $(LIBRETS_LIBS)

clean:
	$(RM) *.o $(LIBRETS_MANAGED_DLL) $(LIBRETS_UNMANAGED_DLL) $(SAMPLE_EXE)
	$(RM) -rf bin

distclean: clean
	$(RM) $(GENERATED_SRC) librets_wrap*

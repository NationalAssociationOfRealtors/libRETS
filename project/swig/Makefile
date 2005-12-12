
SWIG = swig
CSHARP_WRAP = csharp/librets_wrap.cpp

RUBY = ruby
RUBY_MAKEFILE = ruby/Makefile
RUBY_WRAP = ruby/librets_wrap.cpp

PERL = perl
PERL_MAKEFILE = perl/Makefile
PERL_MAKEFILE_PL = perl/Makefile.PL
PERL_WRAP = perl/librets_wrap.cpp

PYTHON = python
PYTHON_WRAP = python/librets_wrap.cpp

SWIG_FILES = librets.i auto_ptr_release.i

all: csharp ruby perl python

csharp: $(CSHARP_WRAP)
	$(MAKE) -C csharp

$(CSHARP_WRAP): $(SWIG_FILES)
	$(SWIG) -c++ -csharp -namespace librets -o $(CSHARP_WRAP) librets.i

ruby: $(RUBY_WRAP) $(RUBY_MAKEFILE)
	$(MAKE) -C ruby

$(RUBY_MAKEFILE): $(RUBY_WRAP)
	cd ruby ; $(RUBY) extconf.rb --with-librets-config=../../../librets-config-inplace

$(RUBY_WRAP): $(SWIG_FILES)
	$(SWIG) -c++ -ruby -o $(RUBY_WRAP) librets.i

perl: $(PERL_WRAP) $(PERL_MAKEFILE)
	$(MAKE) -C perl

$(PERL_WRAP): $(SWIG_FILES)
	$(SWIG) -c++ -perl -o $(PERL_WRAP) librets.i

$(PERL_MAKEFILE):
	cd perl; $(PERL) Makefile.PL

python: $(PYTHON_WRAP)
	$(MAKE) -C python

$(PYTHON_WRAP): $(SWIG_FILES)
	$(SWIG) -c++ -python -o $(PYTHON_WRAP) librets.i

clean-csharp:
	$(MAKE) -C csharp distclean

clean-ruby:
	$(MAKE) -C ruby distclean
	$(RM) $(RUBY_WRAP)

clean-perl:
	$(MAKE) -C perl distclean
	$(RM) $(PERL_WRAP)

clean-python:
	$(MAKE) -C python distclean
	$(RM) $(PYTHON_WRAP)

clean: clean-csharp clean-ruby clean-perl clean-python

.PHONY: csharp ruby perl python \
	clean clean-csharp clean-ruby clean-perl clean-python

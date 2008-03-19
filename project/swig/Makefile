
SWIG = swig

CSHARP_WRAP = csharp/librets_wrap.cpp
CSHARP_LIB_DIR = csharp/lib

RUBY = ruby
RUBY_MAKEFILE = ruby/Makefile
RUBY_WRAP = ruby/librets_wrap.cpp

PERL = perl
PERL_MAKEFILE = perl/Makefile
PERL_MAKEFILE_PL = perl/Makefile.PL
PERL_WRAP = perl/librets_wrap.cpp

PYTHON = python
PYTHON_WRAP = python/librets_wrap.cpp

PHP4 = php
PHP4_WRAP = php4/librets_wrap.cpp

SWIG_FILES = librets.i auto_ptr_release.i

# Perl isn't handling vectors of object ponters correctly
default: csharp ruby python

all: default perl

SWIG_CSHARP = $(SWIG) -c++ -csharp -namespace librets -o $(CSHARP_WRAP) \
	-outdir $(CSHARP_LIB_DIR) -Ilib/csharp librets.i

csharp: $(CSHARP_WRAP)
	$(MAKE) -C csharp

$(CSHARP_WRAP): $(SWIG_FILES) lib/csharp/std_vector.i
	$(RM) $(CSHARP_LIB_DIR)/*.cs
	$(SWIG_CSHARP)

ruby: $(RUBY_WRAP) $(RUBY_MAKEFILE)
	$(MAKE) -C ruby

$(RUBY_MAKEFILE): $(RUBY_WRAP) ruby/extconf.rb
	cd ruby ; $(RUBY) extconf.rb --with-librets-config=../../../librets-config-inplace

SWIG_RUBY = $(SWIG) -c++ -ruby -o $(RUBY_WRAP) -module librets_native \
    -Ilib/ruby librets.i
$(RUBY_WRAP): $(SWIG_FILES) 
	$(SWIG_RUBY)

perl: $(PERL_WRAP) $(PERL_MAKEFILE)
	$(MAKE) -C perl

$(PERL_WRAP): $(SWIG_FILES)
	$(SWIG) -c++ -perl -o $(PERL_WRAP) librets.i

$(PERL_MAKEFILE):
	cd perl; $(PERL) Makefile.PL

$(PHP4_WRAP): $(SWIG_FILES)
	$(SWIG) -Ilib/php4 -c++ -php -o $(PHP4_WRAP) librets.i

php: php4

php4: $(PHP4_WRAP)
	$(MAKE) -C php4

clean-php4:
	$(MAKE) -C php4 distclean

python: $(PYTHON_WRAP)
	$(MAKE) -C python

$(PYTHON_WRAP): $(SWIG_FILES)
	$(SWIG) -c++ -python -o $(PYTHON_WRAP) librets.i

clean-csharp:
	$(MAKE) -C csharp distclean

clean-ruby:
ifneq "$(wildcard $(RUBY_MAKEFILE))" ""
	$(MAKE) -C ruby distclean
endif
	$(RM) $(RUBY_WRAP)

clean-perl:
	$(MAKE) -C perl distclean
	$(RM) $(PERL_WRAP)

clean-python:
	$(MAKE) -C python distclean
	$(RM) $(PYTHON_WRAP)

clean: clean-csharp clean-ruby clean-python

clean-all: clean clean-perl

.PHONY: default all \
	csharp ruby perl python \
	clean clean-all clean-csharp clean-ruby clean-perl clean-python

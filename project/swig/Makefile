
SWIG = swig
CSHARP_WRAP = csharp/librets_wrap.cpp

RUBY = ruby
RUBY_MAKEFILE = ruby/Makefile
RUBY_WRAP = ruby/librets_wrap.cpp

PERL = perl
PERL_MAKEFILE = perl/Makefile
PERL_MAKEFILE_PL = perl/Makefile.PL
PERL_WRAP = perl/librets_wrap.cpp

all: csharp ruby perl

csharp: $(CSHARP_WRAP)
	$(MAKE) -C csharp

$(CSHARP_WRAP): librets.i
	$(SWIG) -c++ -csharp -namespace librets -o $(CSHARP_WRAP) librets.i

ruby: $(RUBY_WRAP) $(RUBY_MAKEFILE)
	$(MAKE) -C ruby

$(RUBY_MAKEFILE): $(RUBY_WRAP)
	cd ruby ; $(RUBY) extconf.rb --with-librets-config=../../../librets-config-inplace

$(RUBY_WRAP): librets.i
	$(SWIG) -c++ -ruby -o $(RUBY_WRAP) librets.i

perl: $(PERL_WRAP) $(PERL_MAKEFILE)
	$(MAKE) -C perl

$(PERL_WRAP): librets.i
	$(SWIG) -c++ -perl -o $(PERL_WRAP) librets.i

$(PERL_MAKEFILE):
	cd perl; $(PERL) Makefile.PL

clean-csharp:
	$(MAKE) -C csharp distclean

clean-ruby:
	$(MAKE) -C ruby distclean
	$(RM) $(RUBY_WRAP)

clean-perl:
	$(MAKE) -C perl distclean
	$(RM) $(PERL_WRAP)

clean: clean-csharp clean-ruby clean-perl

.PHONY: csharp ruby perl clean clean-csharp clean-ruby clean-perl


SWIG = swig
CSHARP_WRAP = csharp/librets_wrap.cpp
RUBY = ruby
RUBY_MAKEFILE = ruby/Makefile
RUBY_WRAP = ruby/librets_wrap.cpp

all: csharp ruby

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

clean:
	$(MAKE) -C csharp distclean
	$(MAKE) -C ruby distclean
	$(RM) $(RUBY_WRAP)

.PHONY: csharp ruby
